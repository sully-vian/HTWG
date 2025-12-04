#include <Arduino_LSM6DSOX.h>
#include <WiFiNINA.h>

void setup() {

    pinMode(LEDB, OUTPUT);
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);

    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1)
            ;
    }
}

int temp_deg = 0; // default value

void loop() {
    if (IMU.temperatureAvailable()) {

        IMU.readTemperature(temp_deg);

        Serial.print("LSM6DSOX Temp = ");
        Serial.print(temp_deg - 3);
        Serial.println(" °C");

        temp_deg = temp_deg - 3;

        if (temp_deg >= 32) { // red
            digitalWrite(LEDR, HIGH);
            digitalWrite(LEDG, LOW);
            digitalWrite(LEDB, LOW);
        } else if (temp_deg > 20 && temp_deg < 32) { // green
            digitalWrite(LEDR, LOW);
            digitalWrite(LEDG, HIGH);
            digitalWrite(LEDB, LOW);
        } else { // blue
            digitalWrite(LEDR, LOW);
            digitalWrite(LEDG, LOW);
            digitalWrite(LEDB, HIGH);
        }
    }
}
