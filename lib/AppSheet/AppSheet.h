#ifndef APPSHEET_H
#define APPSHEET_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

class AppSheet {
  private:
    String appId;
    String accessKey;
    String getBaseUrl(const char* tableName);
    String httpPostRequest(const char* url, const String& payloadString);

  public:
    void begin(const char* APP_ID, const char* ACCESS_KEY);
    JsonDocument getItems(const char* tableName, const char* selector);
    void addItems(const char* tableName, JsonDocument& newItems);
    JsonDocument updateItem(const char* tableName, JsonDocument& updatedItem);
    JsonDocument setItem(const char* tableName, JsonDocument& newItem);
    void deleteItem(const char* tableName, JsonDocument& targetItem);
};

#endif