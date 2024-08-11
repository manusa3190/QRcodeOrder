#include "WiFiManager.h"

WiFiManager::WiFiManager() : _isConnected(false), apModeActive(false), server(80) {

}

void WiFiManager::begin() {
    Serial.println("WiFiManager begin!");
    preferences.begin("WiFiCreds", false);
    ssid = preferences.getString("ssid", "");
    password = preferences.getString("password", "");

    if (ssid != "" && password != "") {
        connectToWiFi();
    }
}

void WiFiManager::update() {
    if (WiFi.status() != WL_CONNECTED && ssid != "" && password != "") {
        connectToWiFi();
    }
    _isConnected = (WiFi.status() == WL_CONNECTED);
}

void WiFiManager::connectToWiFi() {
    Serial.println("Attempting to connect to WiFi...");
    Serial.println("SSID: " + ssid);

    WiFi.begin(ssid.c_str(), password.c_str());
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
        delay(1000);
        Serial.print("Attempt ");
        Serial.print(attempts + 1);
        Serial.println(" failed. Retrying...");
        attempts++;
    }
    _isConnected = (WiFi.status() == WL_CONNECTED);
    if (_isConnected) {
        Serial.println("Connected to WiFi!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Failed to connect to WiFi.");
    }
}

void WiFiManager::drawWiFiIcon() {
    int32_t rssi = WiFi.RSSI();
    String strongness = "圏外";
    if(_isConnected){
        if (rssi > -55) strongness = "強";
        else if (rssi > -70) strongness = "中";
        else if (rssi > -85) strongness = "弱";
    }
    CoreS3.Display.setTextSize(1);
    CoreS3.Display.setTextDatum(TL_DATUM);
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.drawString(strongness, 290, 15);
}

bool WiFiManager::handleTouch(int x, int y) {
    // アイコンがタッチされたかチェック
    if (x >= ICON_X && x < ICON_X + ICON_WIDTH &&
        y >= ICON_Y && y < ICON_Y + ICON_HEIGHT) {
        showWiFiDialog();
        return true;
    }
    return false;
}

void WiFiManager::startAPMode() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("M5StackCoreS3", "");
    IPAddress myIP = WiFi.softAPIP();
    
    server.on("/", HTTP_GET, [this]() {
        // WiFiスキャンを実行
        int n = WiFi.scanNetworks();
        Serial.println("Scan done");
        if (n == 0) {
            Serial.println("No networks found");
        } else {
            Serial.print(n);
            Serial.println(" networks found");
        }

        // HTMLフォームを生成
        String html = "<html><body>";
        html += "<h1>M5Stack Core S3 WiFi Setup</h1>";
        html += "<form method='post' action='/connect'>";
        html += "SSID: <select name='ssid'>";
        for (int i = 0; i < n; ++i) {
            html += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
        }
        html += "</select><br>";
        html += "Password: <input type='password' name='password'><br>";
        html += "<input type='submit' value='Connect'>";
        html += "</form></body></html>";
        server.send(200, "text/html", html);
    });

    Serial.println("Server routes set up");

    server.on("/connect", HTTP_POST, [this]() {
        String newSSID = server.arg("ssid");
        String newPassword = server.arg("password");
        
        // 新しい認証情報を保存
        preferences.putString("ssid", newSSID);
        preferences.putString("password", newPassword);
        
        String response = "<html><body>";
        response += "<h1>WiFi Settings Updated</h1>";
        response += "SSID: " + newSSID + "<br>";
        response += "The device will now restart and attempt to connect to the new network.";
        response += "</body></html>";
        server.send(200, "text/html", response);
        
        delay(2000);
        ESP.restart(); // デバイスを再起動
    });

    server.onNotFound([this]() {
        Serial.println("Received request for unknown page");
        String message = "File Not Found\n\n";
        message += "URI: ";
        message += server.uri();
        message += "\nMethod: ";
        message += (server.method() == HTTP_GET) ? "GET" : "POST";
        message += "\nArguments: ";
        message += server.args();
        message += "\n";
        for (uint8_t i = 0; i < server.args(); i++) {
            message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
        }
        server.send(404, "text/plain", message);
    });

    server.begin();
    apModeActive = true;

    // APモードが開始されたことを画面に表示
    CoreS3.Display.fillScreen(BLACK);
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setTextDatum(MC_DATUM);
    CoreS3.Display.drawString("AP Mode Active", CoreS3.Display.width()/2, CoreS3.Display.height()/2 - 20);
    CoreS3.Display.drawString("SSID: M5StackCoreS3", CoreS3.Display.width()/2, CoreS3.Display.height()/2 + 20);
    CoreS3.Display.drawString("IP: " + myIP.toString(), CoreS3.Display.width()/2, CoreS3.Display.height()/2 + 60);
}

void WiFiManager::handleClient() {
    if (apModeActive) {
        server.handleClient();
    }
}

void WiFiManager::showWiFiDialog() {
    // ダイアログの背景を描画
    CoreS3.Display.fillRect(60, 60, 200, 120, DARKGREY);
    CoreS3.Display.drawRect(60, 60, 200, 120, WHITE);

    // ダイアログのタイトルを描画
    CoreS3.Display.setTextSize(1);
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setTextDatum(TC_DATUM);
    CoreS3.Display.drawString("WiFi設定", 160, 70);

    // 現在の接続状態を表示
    CoreS3.Display.setTextDatum(TL_DATUM);
    if (_isConnected) {
        CoreS3.Display.drawString("接続中: " + ssid, 70, 90);
    } else {
        CoreS3.Display.drawString("未接続", 70, 90);
    }

    // ボタンを描画
    CoreS3.Display.setTextDatum(MC_DATUM);

    CoreS3.Display.fillRect(70, 140, 80, 30, BLUE);
    CoreS3.Display.drawRect(70, 140, 80, 30, WHITE);
    CoreS3.Display.drawString("閉じる", 70+80/2, 140+30/2);

    CoreS3.Display.fillRect(170, 140, 80, 30, GREEN);
    CoreS3.Display.drawRect(170, 140, 80, 30, WHITE);
    CoreS3.Display.drawString("接続設定",170+80/2, 140+30/2);

    // ユーザーの入力を待つ
    while (true) {
        CoreS3.update();
        if (CoreS3.Touch.getCount()) {
            auto touch = CoreS3.Touch.getDetail();
            if (touch.y >= 140 && touch.y < 170) {
                if (touch.x >= 70 && touch.x < 150) {
                    // 「閉じる」ボタンが押された
                    return;
                } else if (touch.x >= 170 && touch.x < 250) {
                    // 「接続設定」ボタンが押された
                    startAPMode();
                    break;
                }
            }
        }
    }
}

