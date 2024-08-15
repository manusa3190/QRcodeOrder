#include "AppController.h"
#include <WiFiManager.h>
#include <AppSheet.h>
#include <secrets.h>

WiFiManager wifiManager;

AppSheet appsheet;

M5UnitQRCodeI2C qrcode;

AppController::AppController() : currentPage(0) {

}

void AppController::run() {
    Serial.println("AppController run!");

    // appsheetのセットアップ。APP_IDおよびACCESS_KEYはsecret.hに格納している
    appsheet.begin(APP_ID,ACCESS_KEY);

    // QRコードスキャナーのセットアップ。M5stack coreS3ではI2CはSDL21、SCA:22
    Wire.begin();
    delay(1000);
    while (!qrcode.begin(&Wire, UNIT_QRCODE_ADDR, 21, 22, 100000U)) {
        Serial.println("Unit QRCode I2C Init Fail");
        delay(1000);
    }

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
        }
    }
}