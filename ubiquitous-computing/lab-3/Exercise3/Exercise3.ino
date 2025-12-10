#include <Arduino_LSM6DSOX.h>

void setup() {
    Serial.begin(9600);
    if (!IMU.begin()) {
        Serial.println("Failed to init IMU!");
        while (1)
            ;
    }
}

void loop() {
    float tempC; // use a float for better precision

    // read temperature and write to serial port
    if (IMU.temperatureAvailable()) {
        IMU.readTemperatureFloat(tempC);
        Serial.println(tempC);
    }
    delay(100);
}
