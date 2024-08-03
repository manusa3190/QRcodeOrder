#include "Order.h"
#include <Button.h>

Order::Order() : currentItemPage(0) {
    items = {
        {1, "黒色ボールペン", 100},
        {2, "メモパッド", 200},
        {3, "単三電池 24本入り", 1200},
        {4, "ホワイトボードマーカー", 300},
        {5, "付箋", 150},
        {6, "クリップ", 80},
        {7, "ホッチキス", 500},
        {8, "消しゴム", 50},
        {9, "シャープペンシル", 300},
        {10, "定規", 100},
        {11, "カッターナイフ", 200},
        {12, "ハサミ", 400},
        {13, "のり", 150},
        {14, "テープ", 100},
        {15, "ファイル", 250},
        {16, "クリアファイル", 80},
        {17, "ノート", 200},
        {18, "カレンダー", 500},
        {19, "電卓", 1000},
        {20, "蛍光ペン", 150}
    };
    setupButtons();
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
    drawPage();

    while (true) {
        M5.update();

        if (M5.Touch.getCount() > 0) {
            auto touch = M5.Touch.getDetail();

            if(touch.x > 280){
                scrollUpButton.isPressed(touch.x, touch.y);
                scrollDownButton.isPressed(touch.x, touch.y);                
            }else if(touch.y < 200) {
                // アイテムの選択処理
                int index = touch.y / 50;
                if (index < items.size()) {
                    showQRCode(items[index].id);
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
        CoreS3.Display.drawString(item.name, 10, y);
        CoreS3.Display.drawString("¥" + String(item.price), 200, y);
        y += 30;
    }

    // スクロールボタンの描画
    scrollUpButton.draw();
    scrollDownButton.draw();
}

void Order::showQRCode(int id) {
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
    CoreS3.Display.qrcode(String(id), mx+p,my+p,std::min(dialogW,dialogH)-p*2,6);

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