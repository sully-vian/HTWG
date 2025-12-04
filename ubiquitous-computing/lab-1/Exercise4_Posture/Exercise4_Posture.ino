#include <Arduino_LSM6DSOX.h>
#include <MadgwickAHRS.h>
#include <WiFiNINA.h>

Madgwick filter;
void setup() {
    filter.begin(104);
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1)
            ;
    }
}

int count = 0;
int target_angle = 90;
int threshold = 20;

void loop() {
    float ax, ay, az;

    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(ax, ay, az);
    }

    float gx, gy, gz;
    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gx, gy, gz);
    }

    filter.updateIMU(gx, gy, gz, ax, ay, az);
    float pitch = filter.getPitch();

    bool ok = (target_angle - threshold <= pitch) &&
              (pitch <= target_angle + threshold);

    if (!ok) {
        digitalWrite(LEDR, HIGH);
    }

    if (count == 100) { // print once every 100 iteration
        Serial.print("pitch = ");
        Serial.print(pitch);
        Serial.print("° : ");
        Serial.println(ok ? "OK" : "NICHT OK");
        count = 0;
    }

    count++;

    delay(5);
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDG, LOW);
}
