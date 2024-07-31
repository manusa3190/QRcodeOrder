#include "AppController.h"

AppController::AppController() : currentPage(0) {}

void AppController::run() {
    while (true) {
        switch (currentPage) {
            case 0:
                currentPage = home.show();
                break;
            case 1:
                currentPage = order.show();
                break;
            case 2:
                currentPage = delivery.show();
                break;
        }
    }
}