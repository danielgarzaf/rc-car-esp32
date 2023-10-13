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

void Driver::SetDirectionMotorA(MotorDirection direction) {
    m_MotorA.direction = direction;
    digitalWrite(pins.in1, (u8)direction & BIT1);
    digitalWrite(pins.in2, (u8)direction & BIT0);
}

void Driver::SetDirectionMotorB(MotorDirection direction) {
    m_MotorB.direction = direction;
    digitalWrite(pins.in3, (u8)direction & BIT1);
    digitalWrite(pins.in4, (u8)direction & BIT0);
}

void Driver::SetSpeedMotorA(u8 speed) {
    m_MotorA.speed = (u8) _clamp<u32>(2 * speed, 0, UINT8_MAX);
    analogWrite(pins.enA, (u8) m_MotorA.speed);
}

void Driver::SetSpeedMotorB(u8 speed) {
    m_MotorB.speed = (u8) _clamp<u32>(2 * speed, 0, UINT8_MAX);
    analogWrite(pins.enB, (u8) m_MotorB.speed);
}

u8 Driver::SpeedA() const {
    return m_MotorA.speed;
}

u8 Driver::SpeedB() const {
    return m_MotorB.speed;
}

MotorDirection Driver::DirectionA() const {
    return m_MotorA.direction;
}

MotorDirection Driver::DirectionB() const {
    return m_MotorB.direction;
}
