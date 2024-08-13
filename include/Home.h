#ifndef HOME_H
#define HOME_H

#include <M5CoreS3.h>
#include <Button.h>
#include <WiFiManager.h>

class Home {
public:
    Home();
    int show();

private:
    struct OrderItem {
        String status;
        String date;
        String name;
    };
    WiFiManager wifiManager;
    static const int ORDER_HISTORY_SIZE = 4;
    OrderItem orderHistory[ORDER_HISTORY_SIZE];
    Button newOrderButton;
    Button deliveryButton;
    void drawPage();
    void fetchOrderHistory();
    void setupButtons();
    int nextPage; 
};

#endif