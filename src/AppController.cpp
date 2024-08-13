#include "AppController.h"
#include <WiFiManager.h>
#include <AppSheet.h>
#include <secrets.h>

AppSheet appsheet;
WiFiManager wifiManager;

AppController::AppController() : currentPage(0) {

}

void AppController::run() {
    Serial.println("AppController run!");

    appsheet.begin(APP_ID,ACCESS_KEY);

    while (true) {
        CoreS3.update();

        switch (currentPage) {
            case 0:
                currentPage = home.show();
                break;
            case 1:
                currentPage = order.show();
                break;
            case 2:
                currentPage = delivery.show();
                break;
            // case 3:
            //     currentPage = setting.show();
        }
    }
}