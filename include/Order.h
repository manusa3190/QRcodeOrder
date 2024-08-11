#ifndef ORDER_H
#define ORDER_H

#include <M5CoreS3.h>
#include <vector>
#include <Button.h>
#include <WiFiManager.h>
#include <AppSheet.h>

class Order {
public:
    Order();
    int show();

private:
    struct Item {
        int _RowNumber;
        String RowID;
        String equipmentCode;
        String equipmentName;
        String storePlace;
        int price;
        String orderNumber;
        String supplierName;
        String RelatedOrderItems;
    };

    WiFiManager wifiManager;

    AppSheet appsheet;

    std::vector<Item> items;
    void drawPage();
    void showQRCode(String id);
    void setupButtons();
    void scrollUp();
    void scrollDown();

    void fetchItemsFromAppSheet();

    Button scrollUpButton;
    Button scrollDownButton;
    int currentItemPage;  // アイテムリストの現在のページ
};

#endif