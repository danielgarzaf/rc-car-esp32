#include <Arduino.h>
#include <string>
#include "Car.h"
#include "types.h"

#define EN_A_PIN 26
#define EN_B_PIN 25
#define IN_1_PIN 13
#define IN_2_PIN 12
#define IN_3_PIN 14
#define IN_4_PIN 27
#define BATTERY_PIN 26
#define MAC_ADDRESS "5c:50:d9:e4:a5:c2"

u64 clockSpeedHz = getXtalFrequencyMhz() * 1e6;
Car car( 
        EN_A_PIN,
        EN_B_PIN,
        IN_1_PIN,
        IN_2_PIN,
        IN_3_PIN,
        IN_4_PIN
    );

#ifdef DEBUG
u32 lastForwardThrottle = 0;
u32 lastBackwardThrottle = 0;
u32 lastDirection = 0;
f32 lastAcceleration = 0.0f;
f32 lastDecceleration = 0.0f;
#endif

void waitForController() {
    /* Not using "delay" function to always listen for controller connection */
    const u32 DELAY_MS = 5000;
    while (!car.ControllerConnected()) {
        if (DELAY_MS > (millis() % DELAY_MS))
            log_e("Controller disconnected");
    }
}

void setup() {
    Serial.begin(115200);
    std::string mac(MAC_ADDRESS);
    car.Init(mac);
    Serial.println("Setup complete!");
}

void loop() {
    waitForController();
    u32 lastTimeMS = millis();
#ifdef DEBUG
    if (car.ForwardThrottle() > lastForwardThrottle || car.ForwardThrottle() < lastForwardThrottle) {
        Serial.print("Forward Throttle: " );
        Serial.println(car.ForwardThrottle()); 
        Serial.print("Motor A speed: ");
        Serial.println(car.SpeedA()); 
        Serial.print("Motor B speed: ");
        Serial.println(car.SpeedB()); 
    }
    if (car.BackwardThrottle() > lastBackwardThrottle || car.BackwardThrottle() < lastBackwardThrottle) {
        Serial.print("Backward Throttle: " );
        Serial.println(car.BackwardThrottle()); 
        Serial.print("Motor A speed: ");
        Serial.println(car.SpeedA()); 
        Serial.print("Motor B speed: ");
        Serial.println(car.SpeedB()); 
    }
    if (car.DirectionValue() > lastDirection || car.DirectionValue() < lastDirection) {
        Serial.print("Direction: ");
        Serial.println(car.DirectionValue());
        Serial.print("Motor A speed: ");
        Serial.println(car.SpeedA()); 
        Serial.print("Motor B speed: ");
        Serial.println(car.SpeedB()); 
    }
    if (car.Acceleration() > lastAcceleration || car.Acceleration() < lastAcceleration) {
        Serial.print("Acceleration: ");
        Serial.println(car.Acceleration());
    }
    if (car.Decceleration() > lastDecceleration || car.Decceleration() < lastDecceleration) {
        Serial.print("Decceleration: ");
        Serial.println(car.Decceleration());
    }
    lastForwardThrottle = car.ForwardThrottle();
    lastBackwardThrottle = car.BackwardThrottle();
    lastDirection = car.DirectionValue();
    lastAcceleration = car.Acceleration();
    lastDecceleration = car.Decceleration();
#endif
    u32 currentTimeMS = millis();
    car.Update(currentTimeMS - lastTimeMS);
}

