#include "Car.h"
#include "types.h"
#include <stdint.h>

#define MAX_THROTTLE 1000

static i32 my_max(i32 val1, i32 val2) {
    if (val1 >= val2)
        return val1;
    return val2;
}

static i32 my_min(i32 val1, i32 val2) {
    if (val1 <= val2)
        return val1;
    return val2;
}

static i32 _clamp(i32 val, i32 minVal, i32 maxVal) {
    return my_min(my_max(val, minVal), maxVal);
}

Car::Car(
        u8 throttle,
        u8 battery,
        u8 in1,
        u8 in2,
        u8 in3,
        u8 in4,
        u8 enA,
        u8 enB
    ) {
    pins.throttle = throttle;
    pins.battery = battery;
    // pinMode(pins.throttle, OUTPUT);
    // pinMode(pins.battery, INPUT);

    L298N::MCPins mcPins;
    mcPins.enA = enA;
    mcPins.enB = enB;
    mcPins.in1 = in1;
    mcPins.in2 = in2;
    mcPins.in3 = in3;
    mcPins.in4 = in4;
    m_MotorDriver = L298N::Driver(mcPins);
}

void Car::Init() {
    m_Controller.Init();
}

f32 Car::handleThrottle(f32 throttle, u8 triggerVal1, u8 triggerVal2) {
    if (triggerVal1 > 0) {
        throttle += triggerVal1 * ACCELERATION ;
    }
    else {
        throttle -= DECCELERATION * DECCELERATION_FACTOR;
    }

    if (triggerVal2 > 0) {
        throttle -= triggerVal2 * DECCELERATION;
    } 

    throttle = (f32) _clamp(throttle, 0, MAX_THROTTLE);
    return throttle;
}

void Car::updateThrottle() {
    u8 rightTriggerVal = m_Controller.RightTriggerValue();
    u8 leftTriggerVal = m_Controller.LeftTriggerValue();

    if (m_BackwardThrottle == 0.0f) {
        m_ForwardThrottle = handleThrottle(m_ForwardThrottle, rightTriggerVal, leftTriggerVal);
        m_MotorDriver.SetDirectionMotorA(L298N::Direction::Backward);
        m_MotorDriver.SetDirectionMotorB(L298N::Direction::Backward);
        // analogWrite(pins.throttle, (u8)m_ForwardThrottle);
    }

    if (m_ForwardThrottle == 0.0f) {
        m_BackwardThrottle = handleThrottle(m_BackwardThrottle, leftTriggerVal, rightTriggerVal);
        m_MotorDriver.SetDirectionMotorA(L298N::Direction::Backward);
        m_MotorDriver.SetDirectionMotorB(L298N::Direction::Backward);
        // analogWrite(pins.throttle, (u8)m_BackwardThrottle);
    }
}

void Car::updateMotors() {
    u8 xPos = m_Controller.GetLStickPosition().x;
    updateSteerDirection(xPos);

    f32 linearizedDirection = 0.0f;
    if (m_Direction == CarSteerDirection::Left) {
        linearizedDirection = xPos - INT8_MAX;
    } else {
        linearizedDirection = INT8_MAX + xPos;
    }
    linearizedDirection = _clamp(linearizedDirection, 0, UINT8_MAX);
    m_DirectionValue = linearizedDirection;

    f32 throttlePercent = 0.0f;
    if (m_BackwardThrottle == 0.0f)
        throttlePercent = m_ForwardThrottle / MAX_THROTTLE;
    if (m_ForwardThrottle == 0.0f)
        throttlePercent = m_BackwardThrottle / MAX_THROTTLE;
    
    /* 0-126: start turning left */
    u8 analogValue = (u32) (throttlePercent * (UINT8_MAX - linearizedDirection));
    m_MotorDriver.SetSpeedMotorA(analogValue);

    /* 127 both values for left and right motors will be the same, meaning the
     * car will go forward */

    /* 128 - 255: start turning right */
    analogValue = (u32) (throttlePercent * linearizedDirection);
    m_MotorDriver.SetSpeedMotorB(analogValue);
}

void Car::updateSteerDirection(u8 xPos) {
    if (m_Direction == CarSteerDirection::Left &&
            !(xPos >= INT8_MAX && xPos <= UINT8_MAX))  {
        m_Direction = CarSteerDirection::Right;
    }
    else if (m_Direction == CarSteerDirection::Right &&
            !(xPos >= 0 && xPos <= INT8_MAX)) {
        m_Direction = CarSteerDirection::Left;
    }
}

void Car::updateControllerBatteryColor() {
    // m_Battery = analogRead(pins.battery);
    const u32 MAX_BATTERY_READ = 1024;
    if (m_Battery <= (MAX_BATTERY_READ / 3)) {
        m_Controller.SetBatteryColor(Colors::Red);
    }
    else if (m_Battery <= (2*MAX_BATTERY_READ / 3)) {
        m_Controller.SetBatteryColor(Colors::Yellow);
    }
    else {
        m_Controller.SetBatteryColor(Colors::Green);
    }
}

void Car::Update() {
    updateThrottle();
    updateMotors();
    updateControllerBatteryColor();
}

f32 Car::ForwardThrottle() const {
    return m_ForwardThrottle;
}

f32 Car::BackwardThrottle() const {
    return m_BackwardThrottle;
}

u32 Car::DirectionValue() const {
    return m_DirectionValue;
}

bool Car::ControllerConnected() {
    return m_Controller.Connected();
}
