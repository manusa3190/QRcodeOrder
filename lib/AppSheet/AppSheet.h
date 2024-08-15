#ifndef APPSHEET_H
#define APPSHEET_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

struct HttpResponse {
    int code;
    JsonDocument result;
};

class AppSheet {
  private:
    String appId;
    String accessKey;
    String getBaseUrl(const char* tableName);
    // String httpPostRequest(const char* url, const String& payloadString);
    HttpResponse httpPostRequest(const char* url, const String& payloadString);

  public:
    void begin(const char* APP_ID, const char* ACCESS_KEY);
    HttpResponse getItems(const char* tableName, const char* selector);
    HttpResponse addItem(const char* tableName, JsonDocument& newItem);
    HttpResponse updateItem(const char* tableName, JsonDocument& updatedItem);
    HttpResponse setItem(const char* tableName, JsonDocument& newItem);
    HttpResponse deleteItem(const char* tableName, JsonDocument& targetItem);
};

#endif