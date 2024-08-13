#include "Setting.h"

Setting::Setting()  {

}

int Setting::show() {
    drawPage();

    while(true){
        M5.update();

        if (M5.Touch.getCount() > 0) {
            auto touch = M5.Touch.getDetail();

        }
    }
}

void Setting::drawPage() {
    int y = 0;

    CoreS3.Display.fillScreen(BLACK);
    CoreS3.Display.setTextColor(WHITE);
    CoreS3.Display.setTextDatum(TL_DATUM);
    CoreS3.Display.setTextSize(2);
    CoreS3.Display.drawString("設定", 5, y);

    y += 35;

    CoreS3.Display.drawLine(0, y, CoreS3.Display.width(),y);

    y += 10;

    CoreS3.Display.setTextSize(1);

    CoreS3.Display.drawString("wifi設定",10,y);
    y +=10;
}

