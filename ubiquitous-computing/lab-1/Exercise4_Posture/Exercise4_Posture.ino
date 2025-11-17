#include <MadgwickAHRS.h>
#include <Arduino_LSM6DSOX.h>
#include <WiFiNINA.h>

Madgwick filter;
void setup() {
  filter.begin(104);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

int count = 0;

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

  if (abs(pitch) < 80 || abs(pitch) > 100) { // handle both ways
    digitalWrite(LEDG, HIGH);
  }

  int temperature_deg = 0;
  if (IMU.temperatureAvailable())
  {
    IMU.readTemperature(temperature_deg);

    if (temperature_deg > 32) {
      digitalWrite(LEDR, HIGH);
    } if (temperature_deg < 25) {
      digitalWrite(LEDB, HIGH);
    }
  }

  if (count==100) {
      Serial.print("pitch = ");
      Serial.println(pitch);
      Serial.print("LSM6DSOX Temperature = ");
      Serial.print(temperature_deg);
      Serial.println(" °C");
      count=0;
  }

  count++;

  delay(5);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDG, LOW); 
}
