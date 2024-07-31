#ifndef HOME_H
#define HOME_H

#include <M5CoreS3.h>
#include <Button.h> 

class Home {
public:
    Home();
    int show();

private:
    struct OrderItem {
        String status;
        String date;
        String name;
    };
    static const int ORDER_HISTORY_SIZE = 4;
    OrderItem orderHistory[ORDER_HISTORY_SIZE];
    Button newOrderButton;
    Button deliveryButton;
    void drawPage();
    void setupButtons();
    int nextPage; 
};

#endif