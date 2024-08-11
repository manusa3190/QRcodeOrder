#include "Order.h"
#include <Button.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "WiFiManager.h"

Order::Order() : currentItemPage(0) {
    fetchItemsFromAppSheet();
    setupButtons();
}

void Order::fetchItemsFromAppSheet() {
    Serial.println("step1");

    WiFiClientSecure client;
    const char* host = "www.appsheet.com";
    const int httpsPort = 443;
    const char* root_ca = "-----BEGIN CERTIFICATE-----\n"
                      "MIIFNzCCBB+gAwIBAgIRAKq/fVF9bGOEEA7WHTMxyLUwDQYJKoZIhvcNAQELBQAw\n"
                      "OzELMAkGA1UEBhMCVVMxHjAcBgNVBAoTFUdvb2dsZSBUcnVzdCBTZXJ2aWNlczEM\n"
                      "MAoGA1UEAxMDV1IzMB4XDTI0MDgwNDAxMDQ0MVoXDTI0MTEwMjAxNTgxNVowFzEV\n"
                      "MBMGA1UEAxMMYXBwc2hlZXQuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB\n"
                      "CgKCAQEAo2MiS6w815fp8Erl98C3SicQXn85jfTwvvFpkMeIkH8FAUm0cNGMY/8V\n"
                      "O5RsfZtr3NM2kCiT8pk4Tem9YA+BQxbB7YSbRlOSwS5RDDV3u9BoV2a11SkNcBko\n"
                      "XLP7NgVeCaAKxyqvIyZq3iu2bLCW8Udw8Vhyqxxe20Q8r18/v3WrV9VEbQAHMrYq\n"
                      "KMkWYq2UvXw2BvxTAbEIFKNbD7gtw0dsRPHamdzF1LwCJoqnSZ6yHM3cMEphW4N7\n"
                      "YQNRFVSL3B3eGXpChjMJ4H0AwAlwK0xc2EUQmuxe8t8iRNUT3/zhn1CxL7oC9w//\n"
                      "o6cMtfoci6M0828vdof5cYnPOuRs1QIDAQABo4ICWDCCAlQwDgYDVR0PAQH/BAQD\n"
                      "AgWgMBMGA1UdJQQMMAoGCCsGAQUFBwMBMAwGA1UdEwEB/wQCMAAwHQYDVR0OBBYE\n"
                      "FETsEE1UE1ckLL1exbgcuF1DO2e9MB8GA1UdIwQYMBaAFMeB9f2OiNkAPE1jolAx\n"
                      "JKDOI/4jMF4GCCsGAQUFBwEBBFIwUDAnBggrBgEFBQcwAYYbaHR0cDovL28ucGtp\n"
                      "Lmdvb2cvcy93cjMvcXI4MCUGCCsGAQUFBzAChhlodHRwOi8vaS5wa2kuZ29vZy93\n"
                      "cjMuY3J0MCkGA1UdEQQiMCCCDGFwcHNoZWV0LmNvbYIQd3d3LmFwcHNoZWV0LmNv\n"
                      "bTATBgNVHSAEDDAKMAgGBmeBDAECATA2BgNVHR8ELzAtMCugKaAnhiVodHRwOi8v\n"
                      "Yy5wa2kuZ29vZy93cjMvXzdTZFJ0ajJXckkuY3JsMIIBBQYKKwYBBAHWeQIEAgSB\n"
                      "9gSB8wDxAHYASLDja9qmRzQP5WoC+p0w6xxSActW3SyB2bu/qznYhHMAAAGRGyG/\n"
                      "8AAABAMARzBFAiEA8vPMjg6Ws+1ZoWKFKQNqGzfsTqIF8IhVGVABCGaoyBkCIAlm\n"
                      "K4txqiOia34ucqIdhpZGybVs23kNBvqTUFxgmxyNAHcAdv+IPwq2+5VRwmHM9Ye6\n"
                      "NLSkzbsp3GhCCp/mZ0xaOnQAAAGRGyG//wAABAMASDBGAiEAvT+oAHZWRTtaSHeH\n"
                      "nLF4CIsXVNeqwNd2cThevXheZ4ECIQDAWUhNRHKb379vwvLajR+FexNYJifNWSTN\n"
                      "4bNHTH2cOzANBgkqhkiG9w0BAQsFAAOCAQEAGPDHr3JRmY+yv0YypzL0QNpOtjiF\n"
                      "Lv83YFwXvLUVdIvtvcSOchRCrwJXwxJTQt77yr/Dwh5P1I+iURZwl5eLzkIFp7uJ\n"
                      "HNKPosar5y1hMPXBDHm1DEYCnRXgMWW2aMN/CJGjQ9sZqDetHFx9jALs24y62vj9\n"
                      "yo48XzBEG9IL8dwAUxTro13nQAON9n/W+VsdksqzcAyuO9x7APfrdYJpBbpg78tc\n"
                      "hSrzOS+/9R6ptgQ70EXplg0C9vaVRMvz/Ihe5GN0Gfhc7kct2jXGpfLtA1gKE4By\n"
                      "CIFnn1TKytd4L1al5pGwAU9PVEuWlm4vhNQL4egtMXLMpkclKVKiPNFaTA==\n"
                      "-----END CERTIFICATE-----\n";
    client.setCACert(root_ca);

    Serial.println("step2");

    if (!client.connect(host, httpsPort)) {
        Serial.println("Connection failed");
        return;
    }

    Serial.println("step3");

    String url = "/api/v2/apps/YOUR_APP_ID/tables/備品マスタ/Action?applicationAccessKey=YOUR_ACCESS_KEY";
    String payload = "{\"Action\":\"Find\",\"Properties\":{\"Locale\":\"ja-JP\",\"Selector\":\"FILTER('備品マスタ', IN('実験室', [格納場所]))\"}}";

    client.println("POST " + url + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Content-Type: application/json");
    client.println("Content-Length: " + String(payload.length()));
    client.println("Connection: close");
    client.println();
    client.println(payload);

    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
            break;
        }
    }

    Serial.println("step3");

    String response = "";
    while (client.available()) {
        response += client.readStringUntil('\n');
    }

    Serial.println("Response: " + response);

    // DynamicJsonDocumentを使用
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, response.c_str());

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        return;
    }

    JsonArray itemsArray = doc["items"].as<JsonArray>();
    for (JsonObject item : itemsArray) {
        int id = item["id"];
        String name = item["name"].as<String>();
        int price = item["price"];
        items.push_back({id, name, price});
    }
}

// Order::Order() : currentItemPage(0) {
//     items = {
//         {1, "黒色ボールペン", 100},
//         {2, "メモパッド", 200},
//         {3, "単三電池 24本入り", 1200},
//         {4, "ホワイトボードマーカー", 300},
//         {5, "付箋", 150},
//         {6, "クリップ", 80},
//         {7, "ホッチキス", 500},
//         {8, "消しゴム", 50},
//         {9, "シャープペンシル", 300},
//         {10, "定規", 100},
//         {11, "カッターナイフ", 200},
//         {12, "ハサミ", 400},
//         {13, "のり", 150},
//         {14, "テープ", 100},
//         {15, "ファイル", 250},
//         {16, "クリアファイル", 80},
//         {17, "ノート", 200},
//         {18, "カレンダー", 500},
//         {19, "電卓", 1000},
//         {20, "蛍光ペン", 150}
//     };
//     setupButtons();
// }

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

int Order::show(WiFiManager& wifiManager) {
    fetchItemsFromAppSheet();

    drawPage();

    while (true) {
        M5.update();

        wifiManager.update();
        wifiManager.handleClient();
        wifiManager.drawWiFiIcon();

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