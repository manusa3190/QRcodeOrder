#ifndef ORDER_H
#define ORDER_H

#include <M5CoreS3.h>
#include <vector>
#include <Button.h>

class Order {
public:
    Order();
    int show();

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

    Button scrollUpButton;
    Button scrollDownButton;
    int currentItemPage;  // アイテムリストの現在のページ
};

#endif