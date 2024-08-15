#include "AppSheet.h"

void AppSheet::begin(const char* appId, const char* accessKey) {
    this->appId = String(appId);
    this->accessKey = String(accessKey);
}

String AppSheet::getBaseUrl(const char* tableName) {
    return "https://www.appsheet.com/api/v2/apps/" + appId + "/tables/" + String(tableName) + "/Action?applicationAccessKey=" + accessKey;
}

HttpResponse AppSheet::httpPostRequest(const char* url, const String& payloadString) {
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    HttpResponse httpResponse;
    int httpResponseCode = http.POST(payloadString);
    httpResponse.code = httpResponseCode;

    switch (httpResponseCode){
        case 200:{
            JsonDocument responseDoc;
            String response = http.getString();
            deserializeJson(responseDoc, response);
            httpResponse.result = responseDoc;
            break;
        }
        case 400:{
            httpResponse.result = "Bad Request";
            break;
        }
        case 403:{
            httpResponse.result = "Forbidden";
            break;
        }
        case 404:{
            httpResponse.result = "Not Found";
            break;
        }
        case 500:{
            httpResponse.result = "Internal Server Error";
            break;
        }
        default:{
            httpResponse.result = "other error";
            break;
        }
    }
    http.end();
    return httpResponse;
}

// OK
HttpResponse AppSheet::addItem(const char* tableName, JsonDocument& newItem) {
    String url = getBaseUrl(tableName);
    JsonDocument payload;
    payload["Action"] = "Add";

    JsonArray rows = payload.createNestedArray("Rows");
    rows.add(newItem.as<JsonObject>());

    JsonObject properties = payload["Properties"].to<JsonObject>();
    properties["Locale"] = "ja-JP";

    String payloadString;
    serializeJson(payload, payloadString);
    return httpPostRequest(url.c_str(), payloadString);
}

// OK
HttpResponse AppSheet::getItems(const char* tableName, const char* selector) {
    String url = getBaseUrl(tableName);
    JsonDocument payload;
    payload["Action"] = "Find";
    JsonObject properties = payload["Properties"].to<JsonObject>();
    properties["Locale"] = "ja-JP";
    properties["Selector"] = selector;

    String payloadString;
    serializeJson(payload, payloadString);
    return httpPostRequest(url.c_str(), payloadString);
}

// OK
HttpResponse AppSheet::updateItem(const char* tableName, JsonDocument& updatedItem) {
    String url = getBaseUrl(tableName);
    JsonDocument payload;
    payload["Action"] = "Edit";

    JsonArray rows = payload.createNestedArray("Rows");
    rows.add(updatedItem.as<JsonObject>());

    JsonObject properties = payload["Properties"].to<JsonObject>();
    properties["Locale"] = "ja-JP";

    String payloadString;
    serializeJson(payload, payloadString);
    return httpPostRequest(url.c_str(), payloadString);
}

HttpResponse AppSheet::deleteItem(const char* tableName, JsonDocument& targetItem) {
    String url = getBaseUrl(tableName);
    JsonDocument payload;
    payload["Action"] = "Delete";

    JsonArray rows = payload.createNestedArray("Rows");
    rows.add(targetItem.as<JsonObject>());

    JsonObject properties = payload["Properties"].to<JsonObject>();
    properties["Locale"] = "ja-JP";

    String payloadString;
    serializeJson(payload, payloadString);
    return httpPostRequest(url.c_str(), payloadString);
}
