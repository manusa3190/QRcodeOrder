#include <Order.h>
#include <Button.h>
#include <AppSheet.h>
#include <secrets.h>

Order::Order() : currentItemPage(0) {
    fetchItemsFromAppSheet();
    setupButtons();
}

void Order::fetchItemsFromAppSheet() {
    // 既にitemsにデータが格納されているなら早期リターン
    if(!items.empty())return;

    Serial.println("start");

    appsheet.begin(APP_ID,ACCESS_KEY);

    const char* TABLE_NAME = "備品マスタ";
    const char* selector = "FILTER('備品マスタ', IN('実験室', [格納場所]))";

    JsonDocument docs = appsheet.getItems(TABLE_NAME,selector);

    Serial.println("fetchItemsFromAppSheet");
    serializeJsonPretty(docs, Serial);

    for (JsonObject item : docs.as<JsonArray>()) {
        Item orderItem;
        orderItem._RowNumber = item["_RowNumber"];
        orderItem.RowID = item["Row ID"].as<String>();
        orderItem.equipmentCode = item["備品コード"].as<String>();
        orderItem.equipmentName = item["備品名"].as<String>();
        orderItem.storePlace = item["格納場所"].as<String>();
        orderItem.price = item["価格"];
        orderItem.orderNumber = item["注文番号"].as<String>();
        orderItem.supplierName = item["発注先名"].as<String>();
        orderItem.RelatedOrderItems = item["Related 注文アイテムs"].as<String>();
        
        items.push_back(orderItem);
    }

}

void Order::setupButtons() {
    scrollUpButton = Button("▲", 280, 40, 30, 30, [this]() { scrollUp(); }, WHITE, BLACK, 1);
    scrollDownButton = Button("▼", 280, 180, 30, 30, [this]() { scrollDown(); }, WHITE, BLACK, 1);
}

void Order::scrollUp() {
    if (currentItemPage > 0) {
        currentItemPage--;
        drawPage();
    }
}

void Order::scrollDown() {
    if ((currentItemPage + 1) * 6 < items.size()) {
        currentItemPage++;
        drawPage();
    }
}

int Order::show() {
    fetchItemsFromAppSheet();
    drawPage();

    // データの確認
    for (const auto& item : items) {
        Serial.println("Item:");
        Serial.println("  _RowNumber: " + String(item._RowNumber));
        Serial.println("  RowID: " + item.RowID);
        Serial.println("  Equipment Code: " + item.equipmentCode);
        Serial.println("  Equipment Name: " + item.equipmentName);
        Serial.println("  Store Place: " + item.storePlace);
        Serial.println("  Price: " + String(item.price));
        Serial.println("  Order Number: " + item.orderNumber);
        Serial.println("  Supplier Name: " + item.supplierName);
        Serial.println("  Related Order Items: " + item.RelatedOrderItems);
        Serial.println();  // 空行で区切る
    }

    while (true) {
        M5.update();

        wifiManager.update();
        wifiManager.handleClient();
        // wifiManager.drawWiFiIcon();

        if (M5.Touch.getCount() > 0) {
            auto touch = M5.Touch.getDetail();

            if(touch.x > 280){
                scrollUpButton.isPressed(touch.x, touch.y);
                scrollDownButton.isPressed(touch.x, touch.y);                
            }else if(touch.y < 200) {
                // アイテムの選択処理
                int index = touch.y / 50;
                if (index < items.size()) {
                    showQRCode(items[index].equipmentCode);
                    drawPage();
                }
            }

            scrollUpButton.update();
            scrollDownButton.update();
        }

        // BtnAが押されたら前の画面に戻る
        if (M5.BtnA.wasPressed()) {
            return 0; // ホームページへ戻る
        }
    }
}

void Order::drawPage() {
    int y = 0;

    CoreS3.Display.fillScreen(BLACK);
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setTextDatum(TL_DATUM);
    CoreS3.Display.setTextSize(2);
    CoreS3.Display.drawString("新規注文",5, y);

    y +=35;

    CoreS3.Display.drawLine(0, y, CoreS3.Display.width(),y);

    y +=15;

    CoreS3.Display.setTextSize(1);
    CoreS3.Display.setTextDatum(ML_DATUM);
    int startIndex = currentItemPage * 6;
    int endIndex = std::min(startIndex + 6, static_cast<int>(items.size()));
    for (int i = startIndex; i < endIndex; i++) {
        const auto& item = items[i];
        CoreS3.Display.drawString(item.equipmentName, 10, y);
        CoreS3.Display.drawString("¥" + String(item.price), 200, y);
        y += 30;
    }

    // スクロールボタンの描画
    scrollUpButton.draw();
    scrollDownButton.draw();
}

void Order::showQRCode(String id) {
    bool shouldClose = false;  // ダイアログを閉じるためのフラグ

    const int mx = 50;
    const int my = 20;
    const int p = 30;
    const int dialogW = CoreS3.Display.width() -mx*2;
    const int dialogH = CoreS3.Display.height()-my*2;

    // ダイアログの描画
    CoreS3.Display.drawRect(mx,my,dialogW,dialogH, WHITE);
    CoreS3.Display.fillRect(mx+2,my+2,dialogW-4,dialogH-4, BLUE);

    CoreS3.Display.setColor(WHITE);
    CoreS3.Display.qrcode(id, mx+p,my+p,std::min(dialogW,dialogH)-p*2,6);

    // 閉じるボタンの作成
    Button closeButton(
        "閉じる", 
        mx + dialogW - 60, my + dialogH - 30, 60, 30, 
        [&shouldClose]() { shouldClose = true;}, 
        RED, WHITE, 1
    );
    
    closeButton.draw();
    
    while (!shouldClose) {  // フラグがfalseの間ループを続ける
        M5.update();
        if (M5.Touch.getCount() > 0) {
            auto touch = M5.Touch.getDetail();
            closeButton.isPressed(touch.x, touch.y);
        }
        closeButton.update();
    }
}