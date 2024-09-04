# USAGE

```cpp main.cpp

#include <AppSheet.h>

AppSheet appsheet;

char* WIFI_SSID = "yourWiFiSSID";
char* WIFI_PASSWORD = "yourWiFiPassword";

char* APP_ID = "yourAppID";
char* ACCESS_KEY = "yourAccessKey"

void setup(){
  Serial.begin(115200);

  // connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");

  // appsheet setup
  appsheet.begin(APP_ID,ACCESS_KEY);

  // GET
  const char* TABLE_NAME = "備品マスタ";
  const char* selector = "FILTER('備品マスタ', IN('実験室', [格納場所]))";

  JsonDocument items = appsheet.getItems(TABLE_NAME,selector);

  serializeJsonPretty(items, Serial);
}

```