#ifndef CAR_H
#define CAR_H

#include <Arduino.h>
#include <L298N.h>
#include <string>
#include "Controller.h"
#include "types.h"

enum CarSteerDirection {
    Left,
    Right,
};

typedef struct CarPins {
    u8 battery = -1;
    L298N::Pins ln298nPins;
} CarPins;

 
class Car {

public:
    CarPins pins;

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
    void Update();

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

    CarSteerDirection m_SteerDirection = CarSteerDirection::Left;

    f32 m_ForwardThrottle = 0.0f;
    f32 m_BackwardThrottle = 0.0f;

    u32 m_DirectionValue = 0.0f;
    u32 m_Battery = 0;

    const f32 DELTA_ACCEL = 0.1f;
    const u8 DECCELERATION_FACTOR = 50;
    f32 m_Decceleration = 0.1f;
    f32 m_Acceleration = 0.1f;

private:
    void updateThrottle();
    void updateMotors();
    void updateControllerBatteryColor();
    void updateSteerDirection(u8 xPos);
    void updateAcceleration();
    void updateDecceleration();

    f32 handleThrottle(f32 throttle, u8 rightTriggerVal, u8 leftTriggerVal);
};

static i32 max_i32(i32 val1, i32 val2) {
    if (val1 >= val2)
        return val1;
    return val2;
}

static i32 min_i32(i32 val1, i32 val2) {
    if (val1 <= val2)
        return val1;
    return val2;
}

static f32 max_f32(f32 val1, f32 val2) {
    if (val1 >= val2)
        return val1;
    return val2;
}

static f32 min_f32(f32 val1, f32 val2) {
    if (val1 <= val2)
        return val1;
    return val2;
}

static f32 _clamp_f32(f32 val, f32 minVal, f32 maxVal) {
    return min_f32(max_f32(val, minVal), maxVal);
}

static i32 _clamp_i32(i32 val, i32 minVal, i32 maxVal) {
    return min_i32(max_i32(val, minVal), maxVal);
}

#endif /* CAR_H */
