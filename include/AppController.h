#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "Home.h"
#include "Order.h"
#include "Delivery.h"

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