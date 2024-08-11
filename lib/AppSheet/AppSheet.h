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
    DynamicJsonDocument getItems(const char* tableName, const char* selector);
    void addItems(const char* tableName, DynamicJsonDocument& newItems);
    DynamicJsonDocument updateItem(const char* tableName, DynamicJsonDocument& updatedItem);
    DynamicJsonDocument setItem(const char* tableName, DynamicJsonDocument& newItem);
    void deleteItem(const char* tableName, DynamicJsonDocument& targetItem);
};

#endif