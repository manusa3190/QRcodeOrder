#include "Home.h"
#include <AppSheet.h>

// AppSheetのインスタンスは外部で定義されていることを想定
extern AppSheet appsheet;

Home::Home() : nextPage(0) {
    // orderHistory[0] = {"注文待", "", "黒色ボールペン"};
    // orderHistory[1] = {"注文済", "2024/7/28", "メモパッド"};
    // orderHistory[2] = {"注文済", "2024/7/28", "単三電池 24本入り"};
    // orderHistory[3] = {"注文済", "2024/7/14", "ホワイトボードマーカー"};
    setupButtons();
}

void Home::setupButtons() {
    newOrderButton = Button(
        "新規注文",
        20, 200, 120, 30,
        [this]() { nextPage = 1; },
        WHITE, WHITE, 1.4
    );

    deliveryButton = Button(
        "納品",
        180, 200, 120, 30,
        [this]() { nextPage = 2; },
        GREEN, WHITE, 1.4
    );
}

int Home::show() {
    wifiManager.update();
    fetchOrderHistory(); // DBからデータを取得してorderHistoryにセット

    drawPage();
    nextPage = 0;

    bool showSomeDialog = false;

    while (true) {
        M5.update();
        
        wifiManager.handleClient();
        wifiManager.update();

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
            Serial.println("ページ遷移!");
            return nextPage;  // ページ遷移
        }
    }
}

void Home::fetchOrderHistory() {
    const char* TABLE_NAME = "注文アイテム";
    const char* selector = "FILTER('注文アイテム', ([場所名]='居室'))";

    JsonDocument docs = appsheet.getItems(TABLE_NAME, selector);

    serializeJsonPretty( docs , Serial);

    JsonArray array = docs.as<JsonArray>();
    int index = 0;

    for (JsonObject item : array) {
        if (index >= ORDER_HISTORY_SIZE) break; // orderHistoryのサイズを超えた場合はループを終了
        orderHistory[index].status = item["status"].as<String>();
        orderHistory[index].date = item["date"].as<String>();
        orderHistory[index].name = item["name"].as<String>();
        index++;
    }

    // 取得データがORDER_HISTORY_SIZEに満たない場合は、残りを空にする
    for (int i = index; i < ORDER_HISTORY_SIZE; i++) {
        orderHistory[i] = {"", "", ""};
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