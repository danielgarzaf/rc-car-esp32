#include "L298N.h"
#include <Arduino.h>

using namespace L298N;

Driver::Driver() {};

Driver::Driver(MCPins p) {
    pins = p;

    // pinMode(pins.enA, OUTPUT);
    // pinMode(pins.enB, OUTPUT);

    // pinMode(pins.in1, OUTPUT);
    // pinMode(pins.in2, OUTPUT);
    // pinMode(pins.in3, OUTPUT);
    // pinMode(pins.in4, OUTPUT);
}

void Driver::SetDirectionMotorA(Direction dir) {
    // digitalWrite(pins.in1, (u8)dir & BIT1);
    // digitalWrite(pins.in2, (u8)dir & BIT0);
}

void Driver::SetDirectionMotorB(Direction dir) {
    // digitalWrite(pins.in3, (u8)dir & BIT1);
    // digitalWrite(pins.in4, (u8)dir & BIT0);
}

void Driver::SetSpeedMotorA(u8 val) {
    // analogWrite(pins.enA, val);
}

void Driver::SetSpeedMotorB(u8 val) {
    // analogWrite(pins.enB, val);
}
