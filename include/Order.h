#ifndef ORDER_H
#define ORDER_H

#include <M5CoreS3.h>
#include <vector>
#include <Button.h>
#include "WiFiManager.h"

class Order {
public:
    Order();
    int show(WiFiManager& wifiManager);

private:
    struct Item {
        int id;
        String name;
        int price;
    };

    std::vector<Item> items;
    void drawPage();
    void showQRCode(int id);
    void setupButtons();
    void scrollUp();
    void scrollDown();

    void fetchItemsFromAppSheet();

    Button scrollUpButton;
    Button scrollDownButton;
    int currentItemPage;  // アイテムリストの現在のページ
};

#endif