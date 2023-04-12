#ifndef L298N_H

#define L298N_H

#include <stdint.h>

namespace L298N {

typedef uint8_t u8;

enum class Direction {
    Forward = 0b10,
    Backward = 0b01
};

typedef struct {
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
} MCPins;

class Driver {

public:
    MCPins pins;

public:
    Driver();
    Driver(MCPins pins);

    void SetDirectionMotorA(Direction dir);
    void SetDirectionMotorB(Direction dir);
    void SetSpeedMotorA(u8 val);
    void SetSpeedMotorB(u8 val);
};
}


#endif /* L298N_H */
