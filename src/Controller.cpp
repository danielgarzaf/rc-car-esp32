#include "Controller.h"
#include <PS4Controller.h>

/************************* PUBLIC START **************************************/
void Controller::Init(const std::string& mac) {
    PS4.begin(mac.c_str());
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

StickPosition Controller::LStickPosition() {
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

bool Controller::DPadUp() {
    return PS4.Up();
}

bool Controller::DPadDown() {
    return PS4.Down();
}

bool Controller::DPadLeft() {
    return PS4.Left();
}

bool Controller::DPadRight() {
    return PS4.Right();
}
/************************* PUBLIC END ****************************************/
