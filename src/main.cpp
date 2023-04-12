#include <Arduino.h>
#include "Car.h"
#include "types.h"

#define EN_A_PIN 33
#define EN_B_PIN 34
#define IN_1_PIN 35
#define IN_2_PIN 35
#define IN_3_PIN 35
#define IN_4_PIN 35
#define BATTERY_PIN 35
#define THROTTLE_PIN 35

Car car( 
        THROTTLE_PIN,
        BATTERY_PIN,
        IN_1_PIN,
        IN_2_PIN,
        IN_3_PIN,
        IN_4_PIN,
        EN_A_PIN,
        EN_B_PIN
    );
u32 lastForwardThrottle = 0;
u32 lastBackwardThrottle = 0;
u32 lastDirection = 0;

void waitForController() {
    u32 timer = 0;
    while (!car.ControllerConnected()) {
        if (timer++ == UINT32_MAX)
            log_e("Controller disconnected");
    }
}

void setup() {
    Serial.begin(115200);
    car.Init();
    Serial.println("Setup complete!");
}

void loop() {
    waitForController();

    car.Update();
#ifdef DEBUG
    if (car.ForwardThrottle() > lastForwardThrottle || car.ForwardThrottle() < lastForwardThrottle) {
        Serial.print("Forward Throttle: " );
        Serial.println(car.ForwardThrottle()); 
    }
    if (car.BackwardThrottle() > lastBackwardThrottle || car.BackwardThrottle() < lastBackwardThrottle) {
        Serial.print("Backward Throttle: " );
        Serial.println(car.BackwardThrottle()); 
    }
    if (car.DirectionValue() > lastDirection || car.DirectionValue() < lastDirection) {
        Serial.print("Direction: ");
        Serial.println(car.DirectionValue());
    }
    delay(100);
#endif

    lastForwardThrottle = car.ForwardThrottle();
    lastDirection = car.DirectionValue();

}

