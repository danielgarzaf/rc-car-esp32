#ifndef L298N_H

#define L298N_H

#include <stdint.h>

namespace L298N {

typedef int8_t    i8;
typedef int16_t  i16;
typedef int32_t  i32;

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef float    f32;
typedef double   f64;

enum class MotorDirection {
    None = 0b00,
    Forward = 0b10,
    Backward = 0b01,
};

typedef struct Motor {
    MotorDirection direction = MotorDirection::None;
    u8 speed = 0;
} Motor;


typedef struct Pins {
    /* Direction control pins for motor A (1 and 2) and motor B (3 and 4)
     * /----------------------------------------\
     * | Input1  | Input2  | Spinning Direction | 
     * | ------  | ------  | ------------------ |
     * | Low(0)  | Low(0)  | Motor OFF          |
     * | ------  | ------  | ------------------ |
     * | High(1) | Low(0)  | Forward            |
     * | ------  | ------  | ------------------ |
     * | Low (0) | High(1) | Backward           |
     * | ------  | ------  | ------------------ |
     * | High(1) | High(1) | Motor OFF          |
     * \---------------------------------------*/
    u8 in1 = -1;
    u8 in2 = -1;
    u8 in3 = -1;
    u8 in4 = -1;

    /* Speed for motor A and motor B */
    u8 enA = -1;
    u8 enB = -1;
} Pins;

class Driver {
private:

public:
    Pins pins;
    Motor m_MotorA;
    Motor m_MotorB;

public:
    Driver();
    Driver(Pins pins);

    void SetDirectionMotorA(MotorDirection direction);
    void SetDirectionMotorB(MotorDirection direction);

    /* Speed ranges from 0-255 (PWM: 0-100%) */
    void SetSpeedMotorA(u8 speed);
    void SetSpeedMotorB(u8 speed);

    u8 SpeedA() const;
    u8 SpeedB() const;

    MotorDirection DirectionA() const;
    MotorDirection DirectionB() const;
};

template<typename T>
static T inline my_max(T val1, T val2) {
    if (val1 >= val2)
        return val1;
    return val2;
}

template<typename T>
static T inline my_min(T val1, T val2) {
    if (val1 <= val2)
        return val1;
    return val2;
}

template<typename T>
static T inline _clamp(T val, T minVal, T maxVal) {
    return my_min(my_max(val, minVal), maxVal);
}

}

#endif /* L298N_H */
