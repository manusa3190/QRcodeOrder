#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <M5CoreS3.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

class WiFiManager {
public:
    WiFiManager();
    void update();
    void drawWiFiIcon();
    bool isConnected() const { return _isConnected; }
    bool handleTouch(int x, int y);
    void showWiFiDialog();
    void startAPMode();
    void handleClient();

private:
    Preferences preferences;
    bool _isConnected;
    void connectToWiFi();
    void drawWiFiSignalStrength();

    // 接続トライ
    unsigned long previousTryMillis = 0;

    // RSSI強度を1秒おきに更新するために使用
    unsigned long previousCheckMillis = 0;

    // アイコンの位置とサイズ
    static const int ICON_X = 290;
    static const int ICON_Y = 15;
    static const int ICON_WIDTH = 30;
    static const int ICON_HEIGHT = 16;

    WebServer server;
    bool apModeActive;
};

#endif