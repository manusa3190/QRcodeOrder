#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <M5CoreS3.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

class WiFiManager {
public:
    WiFiManager();
    void begin();
    void update();
    void drawWiFiIcon();
    bool isConnected() const { return _isConnected; }
    bool handleTouch(int x, int y);
    void showWiFiDialog();
    void startAPMode();
    void handleClient();

private:
    String ssid;
    String password;
    Preferences preferences;
    bool _isConnected;
    void connectToWiFi();
    void drawWiFiSignalStrength();

    // アイコンの位置とサイズ
    static const int ICON_X = 300;
    static const int ICON_Y = 10;
    static const int ICON_WIDTH = 20;
    static const int ICON_HEIGHT = 20;

    WebServer server;
    bool apModeActive;
};

#endif