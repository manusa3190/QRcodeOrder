#include "AppController.h"
#include "WiFiManager.h"

WiFiManager wifiManager;

AppController::AppController() : currentPage(0) {

}

void AppController::run() {
    Serial.println("AppController run!");
    wifiManager.begin();

    while (true) {
        CoreS3.update();

        switch (currentPage) {
            case 0:
                currentPage = home.show(wifiManager);
                break;
            case 1:
                Serial.println("order show");
                currentPage = order.show(wifiManager);
                break;
            case 2:
                currentPage = delivery.show();
                break;
        }
    }
}