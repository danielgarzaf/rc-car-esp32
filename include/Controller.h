#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "types.h"


class Controller {

public:
    void Init();

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

    StickPosition GetLStickPosition();
    StickPosition GetRStickPosition();

    void SetBatteryColor(Colors color);

private:
    Colors m_BatteryColor = Colors::Red;

};

#endif /* CONTROLLER_H */
