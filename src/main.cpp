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
  delay(1000); // これを入れないと処理が追いつかない

  Serial.println("M5Stack initialized");

  M5.Display.setFont(&fonts::efontJA_16);  // または他のサイズを選択
}

void loop() {
  Serial.println("main loop");
  app.run();
}