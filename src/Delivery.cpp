#include "Delivery.h"

Delivery::Delivery() {
    deliveryItems[0] = {"未納品", "黒色ボールペン", false};
    deliveryItems[1] = {"未納品", "メモパッド", false};
    deliveryItems[2] = {"未納品", "単三電池 24本入り", false};
    deliveryItems[3] = {"未納品", "ホワイトボードマーカー", false};
}

int Delivery::show() {
    drawPage();

    while (true) {
        M5.update();
        if (M5.Touch.getCount() > 0) {
            auto touch = M5.Touch.getDetail();
            if (touch.y < 200) {
                int index = touch.y / 50;
                if (index < DELIVERY_ITEMS_SIZE && !deliveryItems[index].delivered) {
                    deliveryItems[index].status = "納品済";
                    deliveryItems[index].delivered = true;
                    drawPage();
                }
            } else if (touch.x > 240) {
                return 0; // ホームページへ戻る
            }
        }
    }
}

void Delivery::drawPage() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("納品");

    for (int i = 0; i < DELIVERY_ITEMS_SIZE; i++) {
        M5.Lcd.printf("%s %s\n", deliveryItems[i].status.c_str(), deliveryItems[i].name.c_str());
    }

    M5.Lcd.drawString("戻る", 280, 220);
}