#include "Controller.h"
#include <PS4Controller.h>


void Controller::Init() {
    PS4.begin("5c:50:d9:e4:a5:c2");
}

bool Controller::RightBumperPressed() {
    return PS4.R1();
}

u8 Controller::RightTriggerValue() {
    return PS4.R2Value();
}

bool Controller::RightStickPressed() {
    return PS4.R3();
}

bool Controller::LeftBumperPressed() {
    return PS4.L1();
}

u8 Controller::LeftTriggerValue() {
    return PS4.L2Value();
}

bool Controller::LeftStickPressed() {
    return PS4.L3();
}

bool Controller::Connected() {
    return PS4.isConnected();
}

StickPosition Controller::GetLStickPosition() {
    StickPosition stickPosition;
    stickPosition.x = PS4.LStickX(); /* Seems like the sticks are reversed? */
    stickPosition.y = PS4.LStickY();
    return stickPosition;
}

StickPosition Controller::GetRStickPosition() {
    StickPosition stickPosition;
    stickPosition.x = PS4.RStickX(); /* Seems like the sticks are reversed? */
    stickPosition.y = PS4.RStickY();
    return stickPosition;
}

void Controller::SetBatteryColor(Colors color) {
    if (color == m_BatteryColor)
        return;
    u8 r = color & (Colors::Red);
    u8 g = color & (Colors::Green);
    u8 b = color & (Colors::Blue);
    PS4.setLed(r, g, b);
}
