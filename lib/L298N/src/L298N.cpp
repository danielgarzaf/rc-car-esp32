#include "L298N.h"
#include <Arduino.h>

using namespace L298N;

Driver::Driver() {};

Driver::Driver(Pins p) {
    pins = p;

    pinMode(pins.enA, OUTPUT);
    pinMode(pins.enA, ANALOG);

    pinMode(pins.enB, OUTPUT);
    pinMode(pins.enB, ANALOG);

    pinMode(pins.in1, OUTPUT);
    pinMode(pins.in2, OUTPUT);
    pinMode(pins.in3, OUTPUT);
    pinMode(pins.in4, OUTPUT);

    analogWrite(pins.enA, 0);
    analogWrite(pins.enB, 0);

    digitalWrite(pins.in1, LOW);
    digitalWrite(pins.in2, LOW);
    digitalWrite(pins.in3, LOW);
    digitalWrite(pins.in4, LOW);
}

void Driver::SetDirectionMotorA(Direction direction) {
    m_Direction = direction;
    digitalWrite(pins.in1, (u8)direction & BIT1);
    digitalWrite(pins.in2, (u8)direction & BIT0);
}

void Driver::SetDirectionMotorB(Direction direction) {
    m_Direction = direction;
    digitalWrite(pins.in3, (u8)direction & BIT1);
    digitalWrite(pins.in4, (u8)direction & BIT0);
}

void Driver::SetSpeedMotorA(u8 speed) {
    m_SpeedA = (f32) _clamp(2 * speed, 0, UINT8_MAX);
    analogWrite(pins.enA, (u8) m_SpeedA);
}

void Driver::SetSpeedMotorB(u8 speed) {
    m_SpeedB = (f32) _clamp(2 * speed, 0, UINT8_MAX);
    analogWrite(pins.enB, (u8) m_SpeedB);
}

u8 Driver::SpeedA() const {
    return m_SpeedA;
}

u8 Driver::SpeedB() const {
    return m_SpeedB;
}
