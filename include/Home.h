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
    struct OrderedItem {
        String ItemCode;
        String OrderState;
        String ItemName;
    };
    static const int ORDER_HISTORY_SIZE = 5;
    OrderedItem orderedItems[ORDER_HISTORY_SIZE];


    unsigned long previousFetchMillis = 0;
    bool fetchOrderHistory(); //更新があればtrue、なければfalse
    void drawOrderedItems();

    WiFiManager wifiManager;
    Button newOrderButton;
    Button deliveryButton;
    void drawPage();

    void setupButtons();
    int nextPage; 
};

#endif