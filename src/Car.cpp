#include "Car.h"
#include "types.h"
#include "utils.h"
#include "esp_sntp.h"
#include <stdint.h>


#define MAX_FORWARD_THROTTLE 1000
#define MAX_BACKWARD_THROTTLE 1000
#define MAX_ACCELERATION 1.0f
#define MAX_DECCELERATION 1.0f

/********************* CONSTRUCTORS START ************************************/
Car::Car(
        u8 enA,
        u8 enB,
        u8 in1,
        u8 in2,
        u8 in3,
        u8 in4
    ) {
    pins.ln298nPins.enA = enA;
    pins.ln298nPins.enB = enB;
    pins.ln298nPins.in1 = in1;
    pins.ln298nPins.in2 = in2;
    pins.ln298nPins.in3 = in3;
    pins.ln298nPins.in4 = in4;
    m_MotorDriver = L298N::Driver(pins.ln298nPins);
}
/********************* CONSTRUCTORS END **************************************/

/************************* PUBLIC START **************************************/
void Car::Init(const std::string& mac) {
    m_Controller.Init(mac);
}

void Car::Update(u32 deltaTimeMS) {
    updateThrottle(deltaTimeMS);
    updateMotors();
    updateAcceleration();
    updateDecceleration();
    // updateControllerBatteryColor();
}

f32 Car::ForwardThrottle() const {
    return m_ForwardThrottle;
}

f32 Car::BackwardThrottle() const {
    return m_BackwardThrottle;
}

f32 Car::Acceleration() const {
    return m_Acceleration;
}

f32 Car::Decceleration() const {
    return m_Decceleration;
}


u32 Car::DirectionValue() const {
    return m_DirectionValue;
}

bool Car::ControllerConnected() {
    return m_Controller.Connected();
}

u8 Car::SpeedA() const {
    return m_MotorDriver.SpeedA();
}

u8 Car::SpeedB() const {
    return m_MotorDriver.SpeedB();
}
/************************* PUBLIC END ****************************************/

/************************* PRIVATE START *************************************/

/* Give priority to the first trigger value over the second */
f32 Car::handleThrottle(
        f32 throttle,
        u8 triggerVal1,
        u8 triggerVal2,
        f32 timeConstant)
    const {
    /* If first trigger has a value, proportionally accelerate */
    if (triggerVal1 > 0) {
        throttle += (triggerVal1 * m_Acceleration * timeConstant);
    }
    /* If second trigger has a value, proportionally deccelerate */
    else if (triggerVal2 > 0) {
        throttle -= (triggerVal2 * m_Decceleration * timeConstant);
    } 
    /* Otherwise, naturally deccelerate */
    else {
        throttle -= (m_Decceleration * DECCELERATION_FACTOR * timeConstant);
    }


    if (m_BackwardThrottle == 0.0f) {
        throttle = (f32) _clamp<i32>(throttle, 0, MAX_FORWARD_THROTTLE);
    }
    if (m_ForwardThrottle == 0.0f) {
        throttle = (f32) _clamp<i32>(throttle, 0, MAX_BACKWARD_THROTTLE);
    }
    return throttle;
}

void Car::updateThrottle(u32 deltaTimeMS) {
    m_CurrentTimeMS += deltaTimeMS;
    if ((m_CurrentTimeMS - m_LastUpdateTimeMS) < THROTTLE_UPDATE_INTERVAL_MS) {
        return;
    }
    m_LastUpdateTimeMS = m_CurrentTimeMS;

    f32 timeConstant = (f32)(m_CurrentTimeMS - m_LastUpdateTimeMS) / (f32)THROTTLE_UPDATE_INTERVAL_MS;
    u8 rightTriggerVal = m_Controller.RightTriggerValue();
    u8 leftTriggerVal = m_Controller.LeftTriggerValue();

    if (m_BackwardThrottle == 0.0f) {
        m_ForwardThrottle = handleThrottle(
                m_ForwardThrottle,
                rightTriggerVal,
                leftTriggerVal,
                timeConstant);
        m_MotorDriver.SetDirectionMotorA(L298N::MotorDirection::Forward);
        m_MotorDriver.SetDirectionMotorB(L298N::MotorDirection::Forward);
    }

    if (m_ForwardThrottle == 0.0f) {
        m_BackwardThrottle = handleThrottle(
                m_BackwardThrottle,
                leftTriggerVal,
                rightTriggerVal,
                deltaTimeMS);
        m_MotorDriver.SetDirectionMotorA(L298N::MotorDirection::Backward);
        m_MotorDriver.SetDirectionMotorB(L298N::MotorDirection::Backward);
    }

    if (m_ForwardThrottle == 0.0f && m_BackwardThrottle == 0.0f) {
        m_MotorDriver.SetDirectionMotorA(L298N::MotorDirection::None);
        m_MotorDriver.SetDirectionMotorB(L298N::MotorDirection::None);
    }

}

void Car::updateMotors() {
    i8 xPos = m_Controller.LStickPosition().x;
    updateSteerDirection(xPos);

    u32 linearizedDirection = map(xPos, INT8_MIN, INT8_MAX, 0, UINT8_MAX);
    m_DirectionValue = linearizedDirection;

    f32 throttlePercent = 0.0f;
    if (m_BackwardThrottle == 0.0f)
        throttlePercent = m_ForwardThrottle / MAX_FORWARD_THROTTLE;
    if (m_ForwardThrottle == 0.0f)
        throttlePercent = m_BackwardThrottle / MAX_BACKWARD_THROTTLE;

    /* 0-126: start turning left */
    u8 analogValue = (u8) (throttlePercent * (UINT8_MAX - linearizedDirection));
    m_MotorDriver.SetSpeedMotorB(analogValue);

    /* 127 both values for left and right motors will be the same, meaning the
     * car will go forward */

    /* 128 - 255: start turning right */
    analogValue = (u8) (throttlePercent * linearizedDirection);
    m_MotorDriver.SetSpeedMotorA(analogValue);
}

void Car::updateSteerDirection(i8 xPos) {
    if (xPos > 0 && xPos <= INT8_MAX)  {
        m_SteerDirection = SteerDirection::Right;
    }
    else if (xPos <= 0 && xPos >= INT8_MIN) {
        m_SteerDirection = SteerDirection::Left;
    }
}

void Car::updateControllerBatteryColor() {
    m_Battery = analogRead(pins.battery);
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

void Car::updateAcceleration() {
    static bool stickyUp = false;
    bool pressing = m_Controller.DPadUp();
    if (pressing && !stickyUp) {
        m_Acceleration += DELTA_ACCEL;
        stickyUp = true;
    }
    else if (!pressing && stickyUp) {
        stickyUp = false;
    }

    static bool stickyDown = false;
    pressing = m_Controller.DPadDown();
    if (pressing && !stickyDown) {
        m_Acceleration -= DELTA_ACCEL;
        stickyDown = true;
    }
    else if (!pressing && stickyDown) {
        stickyDown = false;
    }

    m_Acceleration = _clamp<f32>(m_Acceleration, 0.0f, 1.0f);
}

void Car::updateDecceleration() {
    static bool stickyRight = false;
    bool pressing = m_Controller.DPadRight();
    if (pressing && !stickyRight) {
        m_Decceleration += DELTA_ACCEL;
        stickyRight = true;
    }
    else if (!pressing && stickyRight) {
        stickyRight = false;
    }

    static bool stickyLeft = false;
    pressing = m_Controller.DPadLeft();
    if (pressing && !stickyLeft) {
        m_Decceleration -= DELTA_ACCEL;
        stickyLeft = true;
    }
    else if (!pressing && stickyLeft) {
        stickyLeft = false;
    }
    m_Decceleration = _clamp<f32>(m_Decceleration, DELTA_ACCEL, 1.0f);
}
/*************************** PRIVATE END *************************************/
