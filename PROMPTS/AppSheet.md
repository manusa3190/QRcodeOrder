AppSheetをDBとして使用しています。
使い回しができるよう、AppSheetライブラリを作成したい。

以下のような使い方ができるようAppSheet.hとAppSheet.cppを作成してください

```cpp main.cpp
#include <AppSheet>

AppSheet appsheet

struct Item {
    int id;
    String name;
    int price;
};

void setup(){
  const char* APP_ID = "7b6b4413-ec3a-****-************";
  const char* ACCESS_KEY = "V2-************************";
  appsheet.begin(App_ID, ACCESS_KEY);

  items:Item[];

  const char* tableName = "hogeTable";

  // ADD
  newItems:Item[] = [
    {name:'アイテム1',price:100},
    {name:'アイテム2',price:200},
    {name:'アイテム3',price:300},
  ];
  appsheet.addItems(tableName,newItems); // 各アイテムのidは自動的に付与される

  // GET
  String selector = "FILTER('備品マスタ', IN('実験室', [格納場所]))";
  items = appsheet.getItems(tableName,selector);
  // 結果
  items = [
    {id:"item1",name:'アイテム1',price:100},
    {id:"item2",name:'アイテム2',price:200},
    {id:"item3",name:'アイテム3',price:300},
  ]

  // UPDATE
  newItem:Item = {id:"item1",name:"アイテム1-1"};
  res = appsheet.updateItem(tableName, newItem);
  // 結果
  res = {id:"item1",name:'アイテム1-1',price:100}; // priceは指定していないので書き変わらない

  // SET
  newItem:Item = {id:"item1",name:"アイテム1-2"};
  res = appsheet.setItem(tableName, newItem); // アイテム1があれば更新、なければ追加
  // 結果
  res = {id:"item1",name:'アイテム1-1',price:null}; // priceは指定していないとnullに書き換わる

  // DELETE
  targetItem:Item = {id:"item1"}
  appsheet.deleteItem(tableName,targetItem)
}

```


なお、class AppSheetを作成する時は、以下のコードを参考にすること。
このコードはAppSheetのREST APIを使用して、データを得るものである

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "************";
const char* password = "********";

const char* APP_ID = "7b6b4413-ec3a-****-************";
const char* ACCESS_KEY = "V2-************************";

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");

  const char* TABLE_NAME = "備品マスタ";
  String url = String("https://www.appsheet.com/api/v2/apps/") + APP_ID + "/tables/" + TABLE_NAME + "/Action?applicationAccessKey=" + ACCESS_KEY;

  HTTPClient http;
  http.begin(url);

  http.addHeader("Content-Type", "application/json");

  // JSONペイロードを作成
  DynamicJsonDocument payload(200);
  payload["Action"] = "Find";

  JsonObject properties = payload["Properties"].to<JsonObject>();
  properties["Locale"] = "ja-JP";
  properties["Selector"] = "FILTER('備品マスタ', IN('実験室', [格納場所]))";

  String payloadString;
  serializeJson(payload, payloadString);

  int httpResponseCode = http.POST(payloadString);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response Data: " + response);
  } else {
    Serial.print("Error: ");
    Serial.println(http.errorToString(httpResponseCode));
  }

  http.end();
}

void loop() {
  // Nothing here
}
```