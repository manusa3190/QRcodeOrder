#include "Home.h"
#include <AppSheet.h>

// AppSheetのインスタンスは外部で定義されていることを想定
extern AppSheet appsheet;

Home::Home() : nextPage(0) {
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
    drawPage();
    nextPage = 0;

    bool showSomeDialog = false;

    while (true) {
        M5.update();
        
        wifiManager.handleClient();
        wifiManager.update();

        bool hasUpdate = fetchOrderHistory(); // DBからデータを取得してorderHistoryにセット
        if(hasUpdate)drawOrderedItems();

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

bool Home::fetchOrderHistory() {
    // 前回同期から5秒以上経過していたらfetch
    if(millis() - previousFetchMillis < 5000)return false;
    previousFetchMillis = millis();

    // データの更新があるか判定するため、現在取得しているデータのトップのコードをメモ
    // String oldItemsTopCode = "";
    // if(orderedItems.size() > 0){
    //     oldItemsTopCode = orderedItems[0].ItemCode;
    // }
    String oldItemsTopCode = orderedItems[0].ItemCode;

    const char* selector = "FILTER('注文アイテム', ([場所名]='居室'))";

    HttpResponse res = appsheet.getItems("注文アイテム", selector);

    if(res.code != 200){
        serializeJsonPretty(res.result, Serial);
        return true;
    }

    JsonDocument docs = res.result;

    JsonArray items = docs.as<JsonArray>();
    int index = 0;

    for (JsonObject item : items) {
        if (index >= ORDER_HISTORY_SIZE) break; // orderHistoryのサイズを超えた場合はループを終了
        orderedItems[index].OrderState = item["注文状態"].as<String>();
        orderedItems[index].ItemName = item["備品名"].as<String>();
        orderedItems[index].ItemCode = item["アイテムコード"].as<String>();
        index++;
    }

    // 取得データがORDER_HISTORY_SIZEに満たない場合は、残りを空にする
    for (int i = index; i < ORDER_HISTORY_SIZE; i++) {
        orderedItems[i] = {"", "", ""};
    }

    return orderedItems[0].ItemCode == oldItemsTopCode;
}

void Home::drawOrderedItems(){
    // 画面上にオーダーを描画
    int y = 50;
    CoreS3.Display.setTextSize(1);
    CoreS3.Display.setTextDatum(ML_DATUM);
    int rowHeight = 20;
    for (int i = 0; i < ORDER_HISTORY_SIZE; i++) {
        String status = orderedItems[i].OrderState;

        CoreS3.Display.fillRoundRect(10,y,70,rowHeight, rowHeight/2 ,status=="注文待"? BLUE:TFT_ORANGE);
        CoreS3.Display.drawString(status,               20,y+rowHeight/2);
        CoreS3.Display.drawString(orderedItems[i].ItemName, 90,y+rowHeight/2);
        
        y += rowHeight + 10;
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

    newOrderButton.draw();
    deliveryButton.draw();
}