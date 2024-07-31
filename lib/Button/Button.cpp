#include "Button.h"

Button::Button(
    const String& label,
    int x, int y, int width, int height,
    std::function<void()> onPressed,
    uint32_t color,
    uint32_t textColor,
    float textSize,
    bool autoDraw
) : label(label), x(x), y(y), width(width), height(height),
    onPressed(onPressed), color(color), textColor(textColor),
    textSize(textSize), autoDrawEnabled(autoDraw)
{
    if (autoDrawEnabled) {
        draw();
    }
}

void Button::draw() const {

    int32_t colorValue = isHighlighted? color : ((color & 0xFF000000) | (~color & 0x00FFFFFF));
    CoreS3.Display.fillRect(x, y, width, height, colorValue);
    CoreS3.Display.drawRect(x, y, width, height, colorValue);

    int32_t textColorValue = isHighlighted? textColor : ((textColor & 0xFF000000) | (~textColor & 0x00FFFFFF));
    CoreS3.Display.setTextSize(textSize);
    CoreS3.Display.setTextColor(textColorValue);
    CoreS3.Display.setTextDatum(MC_DATUM);
    CoreS3.Display.drawString(label, x + width / 2, y + height / 2);
}

bool Button::isPressed(int touchX, int touchY) {
    bool pressed = (touchX >= x && touchX < x + width &&
                    touchY >= y && touchY < y + height);
    if (pressed && !isHighlighted) {
        isHighlighted = true;
        pressTime = millis();
        draw();
        if (onPressed) {
            onPressed();
        }
    }
    return pressed;
}

void Button::update() {
    if (isHighlighted && millis() - pressTime > 200) {
        isHighlighted = false;
        draw();
    }
}