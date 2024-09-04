#include "WiFiManager.h"

WiFiManager::WiFiManager() : _isConnected(false), apModeActive(false), server(80) {

}

void WiFiManager::begin() {
    if(WiFi.status() == WL_CONNECTED)return;

    // ssidまたはpasswordが定義されていないなら何もしない
    preferences.begin("WiFiCreds", false);
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");
    if(ssid == "" || password == ""){
        Serial.println("ssid or password is not defined!");
        return;
    }

    Serial.println("ssid: "+ ssid);
    Serial.println("password: "+ password);
    Serial.print("WiFi接続トライ");

    int W = CoreS3.Display.width();
    int H = CoreS3.Display.height();
    CoreS3.Display.fillScreen(BLACK);
    CoreS3.Display.setTextDatum(MC_DATUM);
    CoreS3.Display.setTextSize(2);
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.drawString("WiFi接続中",W/2,H/2-20);
    CoreS3.Display.drawString("SSID: "+ ssid,W/2,H/2+20);    

    WiFi.begin(ssid, password);

    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        count++;
        Serial.print(".");
        if(count>3){
            Serial.println('WiFi接続失敗!!');
            CoreS3.Display.fillRect( (W-240)/2,(H-100)/2, 240,100, BLACK);
            CoreS3.Display.drawString("WiFi接続失敗!!",W/2,H/2);
            break;
        }
    }

}

void WiFiManager::update() {
    // 更新は最低2秒は空ける
    if(millis() - previousTryMillis < 2000)return;
    previousTryMillis = millis();

    // APモードの時はWiFi接続にトライせず、handleClientに徹する
    while(apModeActive){
        handleClient();
    }

    _isConnected = WiFi.status() == WL_CONNECTED;

    // 接続されていない場合は接続トライ
    if(!_isConnected){
        // ただし、ssidまたはpasswordが定義されていないなら何もしない
        preferences.begin("WiFiCreds", false);
        String ssid = preferences.getString("ssid", "");
        String password = preferences.getString("password", "");
        if(ssid == "" || password == ""){
            Serial.println("ssid or password is not defined!");
            return;
        }        

        Serial.println("ssid: "+ ssid);
        Serial.println("password: "+ password);
        Serial.print("try to connect WiFi .");

        WiFi.begin(ssid, password);
    }
    
    String strongness = "圏外";
    if(_isConnected){
        int32_t rssi = WiFi.RSSI();
        if (rssi > -55) strongness = "強";
        else if (rssi > -70) strongness = "中";
        else if (rssi > -85) strongness = "弱";
    }

    // 画面右上に電波強度を表示
    CoreS3.Display.fillRect(ICON_X,ICON_Y,ICON_WIDTH,ICON_HEIGHT,BLACK);

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
        html += "Password: <input name='password'><br>";
        html += "<input type='submit' value='Connect'>";
        html += "</form></body></html>";
        server.send(200, "text/html", html);
    });

    Serial.println("Server routes set up");

    server.on("/connect", HTTP_POST, [this]() {
        String newSSID = server.arg("ssid");
        String newPassword = server.arg("password");

        // 新しい認証情報で接続チェック
        Serial.println("newSSID: " + newSSID);
        Serial.println("newPassword: " + newPassword);
        Serial.print("try to connect WiFi");
        
        // 最大5秒間接続トライ
        WiFi.begin(newSSID, newPassword);
        int count = 0;
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            count++;
            Serial.print(".");
            if(count>5){
                Serial.println("");
                break;
            }
        }

        if(WiFi.status() == WL_CONNECTED){
            Serial.println("newSSID and password is available!");
            // 接続できた場合は、新しい認証情報を保存
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

        }else{
            Serial.println("newSSID and password is NOT available!");
            String response = "<html><body>";
            response += "<h1>WiFi Settings Failured</h1>";
            response += "</body></html>";
            server.send(200, "text/html", response); 
        }

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
    const int W = CoreS3.Display.width();
    const int H = CoreS3.Display.height();
    int x = (W-240)/2;
    int y = (H-150)/2;
    CoreS3.Display.fillRect(x, y, 240, 150, DARKGREY);
    CoreS3.Display.drawRect(x, y, 240, 150, WHITE);

    // ダイアログのタイトルを描画
    CoreS3.Display.setTextSize(1.5);
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setTextDatum(TC_DATUM);
    CoreS3.Display.drawString("WiFi", W/2, y);

    y += 25;

    // 現在の接続状態を表示
    CoreS3.Display.setTextDatum(TL_DATUM);
    String connectStatus = "未接続";
    if (_isConnected) {
        preferences.begin("WiFiCreds", false);
        String ssid = preferences.getString("ssid","");
        connectStatus = "接続中: " + ssid;
    }
    CoreS3.Display.setTextSize(1);
    CoreS3.Display.drawString(connectStatus, x+20, y);

    y += 30;

    CoreS3.Display.setTextSize(1);
    CoreS3.Display.drawString("この画面を閉じるには", x+10, y);
    y += 20;
    CoreS3.Display.drawString("リセットしてください", x+10, y);

    y += 30;

    // ボタンを描画
    CoreS3.Display.setTextSize(1);
    CoreS3.Display.setTextDatum(MC_DATUM);
    CoreS3.Display.fillRect(70, y, 160, 30, BLUE);
    CoreS3.Display.drawRect(70, y, 160, 30, WHITE);
    CoreS3.Display.drawString("接続先変更",70+160/2, y+30/2);

    // ユーザーの入力を待つ
    while (true) {
        CoreS3.update();
        if (CoreS3.Touch.getCount()) {
            auto touch = CoreS3.Touch.getDetail();
            int x = touch.x;
            int y = touch.y;
            if( x>70 && x<70+160 && y > 140 && y < 140+30){
                // 「接続設定」ボタンが押された
                startAPMode();
                break;
            }
        }
    }
}

