#ifndef DELIVERY_H
#define DELIVERY_H

#include <M5CoreS3.h>
#include <Button.h>
#include <vector>

class Delivery {
public:
    Delivery();
    int show();

private:
    void setupKeypad();
    void drawPage();
    void handleKeypadInput(int x, int y, String& inputCode);
    void updateKeypad();
    void displayInputCode(const String& code);
    void drawKeypad();
    void sendDeliveryRequest(const String& janCode);
    void showCompletionDialog();

    std::vector<Button> keypadButtons;
    String inputCode;
};

#endif