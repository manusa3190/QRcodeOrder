#include <M5CoreS3.h>
#include "AppController.h"
#include <efont.h>
#include <efontFontData.h>
#include <nvs_flash.h>

AppController app;

void setup() {
  Serial.begin(115200);

  auto cfg = M5.config();
  M5.begin(cfg);

  int W = CoreS3.Display.width();
  int H = CoreS3.Display.height();
  CoreS3.Display.fillScreen(BLACK);
  CoreS3.Display.setTextDatum(MC_DATUM);
  CoreS3.Display.setTextSize(2.4);
  CoreS3.Display.setTextColor(WHITE);
  CoreS3.Display.drawString("Hello!",W/2,H/2-20);
  CoreS3.Display.drawString("M5Stack coreS3 SE",W/2,H/2+20);

  delay(1000); // これを入れないと処理が追いつかない

  M5.Display.setFont(&fonts::efontJA_16);  // または他のサイズを選択

  Serial.println("M5Stack initialized");
  delay(1000); // 必要ではないが、起動画面を表示させるため
}

void loop() {
  app.run();
}