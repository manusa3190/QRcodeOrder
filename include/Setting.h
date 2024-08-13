#ifndef Setting_H
#define Setting_H

#include <M5CoreS3.h>
#include <vector>
#include <Button.h>
#include <WiFiManager.h>

class Setting {
public:
    Setting();
    int show();

private:
    void drawPage();
};

#endif