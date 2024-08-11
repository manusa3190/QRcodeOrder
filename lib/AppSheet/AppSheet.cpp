#include "AppSheet.h"

void AppSheet::begin(const char* appId, const char* accessKey) {
    this->appId = String(appId);
    this->accessKey = String(accessKey);
}

String AppSheet::getBaseUrl(const char* tableName) {
    return "https://www.appsheet.com/api/v2/apps/" + appId + "/tables/" + String(tableName) + "/Action?applicationAccessKey=" + accessKey;
}

String AppSheet::httpPostRequest(const char* url, const String& payloadString) {
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(payloadString);
    String response = "";
    if (httpResponseCode > 0) {
        response = http.getString();
    } else {
        Serial.print("Error: ");
        Serial.println(http.errorToString(httpResponseCode));
    }
    http.end();
    return response;
}

DynamicJsonDocument AppSheet::getItems(const char* tableName, const char* selector) {
    String url = getBaseUrl(tableName);
    DynamicJsonDocument payload(200);
    payload["Action"] = "Find";
    JsonObject properties = payload["Properties"].to<JsonObject>();
    properties["Locale"] = "ja-JP";
    properties["Selector"] = selector;

    String payloadString;
    serializeJson(payload, payloadString);
    String response = httpPostRequest(url.c_str(), payloadString);

    DynamicJsonDocument responseDoc(1024);
    deserializeJson(responseDoc, response);
    return responseDoc;
}

void AppSheet::addItems(const char* tableName, DynamicJsonDocument& newItems) {
    String url = getBaseUrl(tableName);
    DynamicJsonDocument payload(1024);
    payload["Action"] = "Add";
    payload["Rows"] = newItems;

    String payloadString;
    serializeJson(payload, payloadString);
    httpPostRequest(url.c_str(), payloadString);
}

DynamicJsonDocument AppSheet::updateItem(const char* tableName, DynamicJsonDocument& updatedItem) {
    String url = getBaseUrl(tableName);
    DynamicJsonDocument payload(1024);
    payload["Action"] = "Update";
    payload["Rows"].add(updatedItem);

    String payloadString;
    serializeJson(payload, payloadString);
    String response = httpPostRequest(url.c_str(), payloadString);

    DynamicJsonDocument responseDoc(1024);
    deserializeJson(responseDoc, response);
    return responseDoc;
}

DynamicJsonDocument AppSheet::setItem(const char* tableName, DynamicJsonDocument& newItem) {
    String url = getBaseUrl(tableName);
    DynamicJsonDocument payload(1024);
    payload["Action"] = "Set";
    payload["Rows"].add(newItem);

    String payloadString;
    serializeJson(payload, payloadString);
    String response = httpPostRequest(url.c_str(), payloadString);

    DynamicJsonDocument responseDoc(1024);
    deserializeJson(responseDoc, response);
    return responseDoc;
}

void AppSheet::deleteItem(const char* tableName, DynamicJsonDocument& targetItem) {
    String url = getBaseUrl(tableName);
    DynamicJsonDocument payload(1024);
    payload["Action"] = "Delete";
    payload["Rows"].add(targetItem);

    String payloadString;
    serializeJson(payload, payloadString);
    httpPostRequest(url.c_str(), payloadString);
}
