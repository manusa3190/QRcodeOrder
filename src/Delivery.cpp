#include "Delivery.h"

Delivery::Delivery() {
    setupKeypad();
}

void Delivery::setupKeypad() {
    int buttonWidth = 50;
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
    bool wasTouched = false;

    while (true) {
        M5.update();

        if (M5.Touch.getCount() > 0) {
            auto touch = M5.Touch.getDetail();
            wasTouched = true;
            for (auto& button : keypadButtons) {
                if (button.isPressed(touch.x, touch.y)) {
                    button.update(); // コールバック関数を呼び出す
                    displayInputCode(inputCode);
                    break;
                }
            }
        } else if (wasTouched) {
            // タッチが終了したときの処理
            wasTouched = false;
            drawKeypad(); // ボタンを再描画してリセット
        }

        // BtnAが押されたらホーム画面に戻る
        if (M5.BtnA.wasPressed()) {
            return 0;
        }
    }
}

void Delivery::drawPage() {
    CoreS3.Display.fillScreen(BLACK);
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setTextDatum(TL_DATUM);
    CoreS3.Display.setTextSize(2);
    CoreS3.Display.drawString("納品", 5, 5);

    CoreS3.Display.setTextSize(1);
    CoreS3.Display.drawString("スキャナーまたはキーボードでJANコードを入力ください", 5, 40);

    // 入力コード表示枠の描画
    CoreS3.Display.drawRect(5, 60, CoreS3.Display.width() - 10, 30, WHITE);

    // キーパッドの描画
    drawKeypad();
}

void Delivery::drawKeypad() {
    for (auto& button : keypadButtons) {
        button.draw();
    }
}

void Delivery::displayInputCode(const String& code) {
    CoreS3.Display.fillRect(6, 61, CoreS3.Display.width() - 12, 28, BLACK); // 枠内をクリア
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setTextDatum(TL_DATUM);
    CoreS3.Display.drawString(code, 10, 65);
}



void Delivery::sendDeliveryRequest(const String& janCode) {
    // HTTPリクエストの代わりにシリアル出力
    Serial.println("Delivery request for JAN code: " + janCode);

    // 納品完了ダイアログの表示
    showCompletionDialog();
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

    // ダイアログを消す
    drawPage(); // 元の画面を再描画
}