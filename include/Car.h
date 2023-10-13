#ifndef CAR_H
#define CAR_H

#include <Arduino.h>
#include <L298N.h>
#include <string>
#include "Controller.h"
#include "types.h"

class Car {
    enum class SteerDirection {
        Left,
        Right,
    };

    typedef struct Pins {
        u8 battery = -1;
        L298N::Pins ln298nPins;
    } Pins;

public:
    Pins pins;

public:
    Car(
        u8 enA,
        u8 enB,
        u8 in1,
        u8 in2,
        u8 in3,
        u8 in4
    );

    void Init(const std::string& mac);
    void Update(u32 deltaTimeMS);

    bool ControllerConnected();

    f32 ForwardThrottle() const;
    f32 BackwardThrottle() const;
    f32 Acceleration() const;
    f32 Decceleration() const;

    u32 DirectionValue() const;

    u8 SpeedA() const;
    u8 SpeedB() const;

private:
    Controller m_Controller;

    L298N::Driver m_MotorDriver;

    SteerDirection m_SteerDirection = SteerDirection::Left;

    f32 m_ForwardThrottle = 0.0f;
    f32 m_BackwardThrottle = 0.0f;
    f32 m_Decceleration = 0.1f;
    f32 m_Acceleration = 0.1f;

    u32 m_DirectionValue = 0;
    u32 m_Battery = 0;

    const f32 DELTA_ACCEL = 0.1f;
    const u8 DECCELERATION_FACTOR = 50;

private:
    void updateThrottle(u32 deltaTimeMS);
    void updateMotors();
    void updateControllerBatteryColor();
    void updateAcceleration();
    void updateDecceleration();
    void updateSteerDirection(i8 xPos);

    f32 handleThrottle(f32 throttle, u8 rightTriggerVal, u8 leftTriggerVal, u32 deltaTimeMS);
};

#endif /* CAR_H */
