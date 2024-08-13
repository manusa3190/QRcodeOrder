#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "Home.h"
#include "Order.h"
#include "Delivery.h"

#include "AppSheet.h" // AppSheetのヘッダーをインクルード

extern AppSheet appsheet; // グローバル変数の宣言

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