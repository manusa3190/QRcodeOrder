#include "AppController.h"
#include "WiFiManager.h"

WiFiManager wifiManager;

AppController::AppController() : currentPage(0) {
    wifiManager.begin();
}

void AppController::run() {
    while (true) {
        CoreS3.update();
        wifiManager.update();
        wifiManager.handleClient(); // 新しく追加

        switch (currentPage) {
            case 0:
                currentPage = home.show(wifiManager);
                break;
            case 1:
                currentPage = order.show();
                break;
            case 2:
                currentPage = delivery.show();
                break;
        }
    }
}