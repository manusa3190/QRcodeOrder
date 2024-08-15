#include "Delivery.h"
#include <AppController.h>

Delivery::Delivery() {
    setupKeypad();
}

void Delivery::setupKeypad() {
    int buttonWidth = 64;
    int buttonHeight = 40;
    int startX = 10;
    int startY = 100;
    int padding = 10;

    // ボタンの配置
    std::vector<String> buttonLabels = {
        "7", "8", "9", "DEL",
        "4", "5", "6", "OK",
        "1", "2", "3", "0"
    };

    for (int i = 0; i < buttonLabels.size(); ++i) {
        int x = startX + (i % 4) * (buttonWidth + padding);
        int y = startY + (i / 4) * (buttonHeight + padding);
        
        // 各ボタンの動作を定義
        if (buttonLabels[i] == "DEL") {
            keypadButtons.emplace_back(Button("DEL", x, y, buttonWidth, buttonHeight, [this]() {
                if (!inputCode.isEmpty()) {
                    inputCode.remove(inputCode.length() - 1);
                }
            }, RED, WHITE, 2));
        } else if (buttonLabels[i] == "OK") {
            keypadButtons.emplace_back(Button("OK", x, y, buttonWidth, buttonHeight, [this]() {
                sendDeliveryRequest(inputCode);
            }, GREEN, WHITE, 2));
        } else {
            int value = buttonLabels[i].toInt();
            keypadButtons.emplace_back(Button(buttonLabels[i], x, y, buttonWidth, buttonHeight, [this, value]() {
                inputCode += String(value);
            }, WHITE, BLACK, 2));
        }
    }
}

int Delivery::show() {
    drawPage();
    inputCode = "";

    while (true) {
        M5.update();

        // QRコードスキャナーで読み取られると起きるイベント
        if (qrcode.getDecodeReadyStatus() == 1) {
            // データを取得
            uint8_t buffer[512] = {0};
            uint16_t length     = qrcode.getDecodeLength();
            qrcode.getDecodeData(buffer, length);
            
            // bufferの内容をdataに追加
            String data;
            for (int i = 0; i < length; i++) {
                data += (char)buffer[i]; 
            }

            inputCode = data;
            displayInputCode(inputCode);
        }

        if (M5.Touch.getCount() > 0) {
            auto touch = M5.Touch.getDetail();
            for (auto& button : keypadButtons) {
                if (button.isPressed(touch.x, touch.y)) {
                    displayInputCode(inputCode);
                    break;
                }
            }
        }

        // 全てのボタンのupdate()メソッドを呼び出すことで、ボタンの色をもとに戻す
        for (auto& button : keypadButtons) {
            button.update();
        }

        // BtnAが押されたらホーム画面に戻る
        if (M5.BtnA.wasPressed()) {
            return 0;
        }
    }
}

void Delivery::drawPage() {
    int y = 0;

    CoreS3.Display.fillScreen(BLACK);
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setTextDatum(TL_DATUM);
    CoreS3.Display.setTextSize(2);
    CoreS3.Display.drawString("納品", 5, y);

    y += 35;

    CoreS3.Display.drawLine(0, y, CoreS3.Display.width(),y);

    y += 10;

    CoreS3.Display.setTextSize(1);
    CoreS3.Display.drawString("スキャナーまたはキーボードでJANコードを入力ください", 5, y);

    // 入力コード表示枠はwhile(true)の中で描くため、ここでは描画しない

    // キーパッドの描画
    for (auto& button : keypadButtons) {
        button.draw();
    }
}

void Delivery::displayInputCode(const String& code) {
    const int centerY = 80;
    const int mx = 5;
    const int px = 5;

    // 入力コード表示枠の描画
    CoreS3.Display.drawRect(mx,   centerY-15, CoreS3.Display.width() - mx*2,   15*2, WHITE);
    CoreS3.Display.fillRect(mx+1, centerY-14, CoreS3.Display.width()-(mx+px)*2,14*2, BLACK);

    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setTextDatum(ML_DATUM);
    CoreS3.Display.setTextSize(2);
    CoreS3.Display.drawString(code, mx+px, centerY);
}

void Delivery::sendDeliveryRequest(const String& janCode) {
    // なぜか1回のタップで3回実行されてしまうので、2回目と3回目はスルーする
    loading = !loading;
    if (!loading) return;

    // HTTPリクエストの代わりにシリアル出力
    Serial.println("Delivery request for JAN code: " + janCode);

    JsonDocument newItem6;
    newItem6["アイテムコード"] = "item0006";
    newItem6["注文状態"] = "納品済";

    // 納品完了ダイアログの表示
    HttpResponse res = appsheet.addItem("注文アイテム",newItem6);
    if(res.code == 200){
        showCompletionDialog();
    }else{
        serializeJsonPretty(res.result, Serial);
    }
}

void Delivery::showCompletionDialog() {
    const int dialogW = 240;
    const int dialogH = 100;
    const int dialogX = (CoreS3.Display.width() - dialogW) / 2;
    const int dialogY = (CoreS3.Display.height() - dialogH) / 2 + 50;

    CoreS3.Display.fillRect(dialogX, dialogY, dialogW, dialogH, BLUE);
    CoreS3.Display.drawRect(dialogX, dialogY, dialogW, dialogH, WHITE);

    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setTextDatum(MC_DATUM);
    CoreS3.Display.drawString("納品しました", dialogX + dialogW / 2, dialogY + dialogH / 2);

    delay(2000); // 2秒間表示

    drawPage();
}