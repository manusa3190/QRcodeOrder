#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "Home.h"
#include "Order.h"
#include "Delivery.h"

// AppSheet
#include <AppSheet.h>
extern AppSheet appsheet;

// QRコードリーダー
#include <M5UnitQRCode.h>
#define UNIT_QRCODE_ADDR 0x21
extern M5UnitQRCodeI2C qrcode;

class AppController {
public:
    AppController();
    void run();

private:
    Home home;
    Order order;
    Delivery delivery;
    int currentPage;
};

#endif