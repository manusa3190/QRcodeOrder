#ifndef DELIVERY_H
#define DELIVERY_H

#include <M5CoreS3.h>

class Delivery {
public:
    Delivery();
    int show();

private:
    struct DeliveryItem {
        String status;
        String name;
        bool delivered;
    };
    static const int DELIVERY_ITEMS_SIZE = 4;
    DeliveryItem deliveryItems[DELIVERY_ITEMS_SIZE];
    void drawPage();
};

#endif