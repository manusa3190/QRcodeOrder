#include <M5CoreS3.h>
#include "AppController.h"
#include <efont.h>
#include <efontFontData.h>

AppController app;

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setFont(&fonts::efontJA_16);  // または他のサイズを選択
}

void loop() {
  app.run();
}