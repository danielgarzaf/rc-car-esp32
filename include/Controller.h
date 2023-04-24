#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "types.h"
#include <string>


class Controller {
public:
    void Init(const std::string& mac);

    /* D-Pad */
    bool DPadUp();
    bool DPadDown();
    bool DPadLeft();
    bool DPadRight();

    /* R Buttons */
    bool RightBumperPressed();
    bool RightStickPressed();
    u8 RightTriggerValue();

    /* L Buttons */
    bool LeftBumperPressed();
    bool LeftStickPressed();
    u8 LeftTriggerValue();

    /* Main Face Buttons */
    bool TopMainFaceButtonPressed();
    bool RightMainFaceButtonPressed();
    bool DownMainFaceButtonPressed();
    bool LeftMainFaceButtonPressed();

    bool Connected();

    StickPosition LStickPosition();
    StickPosition GetRStickPosition();

    void SetBatteryColor(Colors color);

private:
    Colors m_BatteryColor = Colors::Red;
};

#endif /* CONTROLLER_H */
