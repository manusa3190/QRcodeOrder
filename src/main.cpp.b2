#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <AppSheet.h>
#include <secrets.h>

AppSheet appsheet;

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");

  appsheet.begin(APP_ID,ACCESS_KEY);

  const char* TABLE_NAME = "備品マスタ";
  const char* selector = "FILTER('備品マスタ', IN('実験室', [格納場所]))";

  DynamicJsonDocument items = appsheet.getItems(TABLE_NAME,selector);

  serializeJsonPretty(items, Serial);

}

void loop() {
  // Nothing here
}