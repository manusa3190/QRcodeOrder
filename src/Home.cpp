#include "Home.h"

Home::Home() : nextPage(0) {
    orderHistory[0] = {"注文待", "", "黒色ボールペン"};
    orderHistory[1] = {"注文済", "2024/7/28", "メモパッド"};
    orderHistory[2] = {"注文済", "2024/7/28", "単三電池 24本入り"};
    orderHistory[3] = {"注文済", "2024/7/14", "ホワイトボードマーカー"};
    wifiManager.begin();
    setupButtons();
}

void Home::setupButtons() {
    newOrderButton = Button(
        "新規注文",
        20, 200, 120, 30,
        [this]() { nextPage = 1; },
        BLUE, WHITE, 1.4
    );

    deliveryButton = Button(
        "納品",
        180, 200, 120, 30,
        [this]() { nextPage = 2; },
        GREEN, WHITE, 1.4
    );
}

int Home::show() {
    drawPage();
    nextPage = 0;

    while (true) {
        M5.update();
        
        wifiManager.update();
        wifiManager.handleClient();
        wifiManager.drawWiFiIcon();

        if (M5.Touch.getCount() > 0) {
            auto touch = M5.Touch.getDetail();

            if (wifiManager.handleTouch(touch.x, touch.y)) {
                // WiFiアイコンがタッチされた場合、ダイアログが表示される
                continue;
            }
            newOrderButton.isPressed(touch.x, touch.y);
            deliveryButton.isPressed(touch.x, touch.y);
        }
        
        newOrderButton.update();
        deliveryButton.update();

        if (nextPage != 0) {
            return nextPage;  // ページ遷移
        }
    }
}

void Home::drawPage() {
    int y = 0;

    CoreS3.Display.fillScreen(BLACK);
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setTextDatum(TL_DATUM);
    CoreS3.Display.setTextSize(2);
    CoreS3.Display.drawString("発注履歴",5, y);

    y +=35;

    CoreS3.Display.drawLine(0, y, CoreS3.Display.width(),y);

    y +=15;

    CoreS3.Display.setTextSize(1);
    CoreS3.Display.setTextDatum(ML_DATUM);
    int rowHeight = 20;
    for (int i = 0; i < ORDER_HISTORY_SIZE; i++) {
        String status = orderHistory[i].status;

        CoreS3.Display.fillRoundRect(10,y,70,rowHeight, rowHeight/2 ,status=="注文待"? BLUE:TFT_ORANGE);
        CoreS3.Display.drawString(status,               20,y+rowHeight/2);
        CoreS3.Display.drawString(orderHistory[i].name, 90,y+rowHeight/2);
        
        y += rowHeight + 10;
    }

    newOrderButton.draw();
    deliveryButton.draw();
}