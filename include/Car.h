#ifndef CAR_H
#define CAR_H

#include <Arduino.h>
#include <L298N.h>
#include "Controller.h"
#include "types.h"

enum CarSteerDirection {
    Left,
    Right,
};

typedef struct CarPins {
    u8 throttle = -1;
    u8 battery = -1;
} CarPins;

 
class Car {

public:
    CarPins pins;

public:
    Car(
        u8 throttle,
        u8 battery,
        u8 in1,
        u8 in2,
        u8 in3,
        u8 in4,
        u8 enA,
        u8 enB
    );

    void Init();
    void Update();

    bool ControllerConnected();

    f32 ForwardThrottle() const;
    f32 BackwardThrottle() const;

    u32 DirectionValue() const;

private:
    Controller m_Controller;

    L298N::Driver m_MotorDriver;

    CarSteerDirection m_Direction = CarSteerDirection::Left;

    f32 m_ForwardThrottle = 0.0f;
    f32 m_BackwardThrottle = 0.0f;

    u32 m_DirectionValue = 0.0f;
    u32 m_Battery = 0;

    const u8 DECCELERATION_FACTOR = 10;
    const f32 DECCELERATION = 0.1f;
    const f32 ACCELERATION = 0.1f;

private:
    void updateThrottle();
    void updateMotors();
    void updateControllerBatteryColor();

    void updateSteerDirection(u8 xPos);

    f32 handleThrottle(f32 throttle, u8 rightTriggerVal, u8 leftTriggerVal);
};

#endif /* CAR_H */
