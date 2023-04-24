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

enum class Direction {
    None = 0b00,
    Forward = 0b10,
    Backward = 0b01,
};

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
    u8 m_SpeedA;
    u8 m_SpeedB;
    Direction m_Direction;

public:
    Driver();
    Driver(Pins pins);

    void SetDirectionMotorA(Direction direction);
    void SetDirectionMotorB(Direction direction);

    /* Speed ranges from 0-255 (PWM: 0-100%) */
    void SetSpeedMotorA(u8 speed);
    void SetSpeedMotorB(u8 speed);

    u8 SpeedA() const;
    u8 SpeedB() const;
};

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

}

#endif /* L298N_H */
