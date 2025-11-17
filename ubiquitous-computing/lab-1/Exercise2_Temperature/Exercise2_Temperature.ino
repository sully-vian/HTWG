#include <Arduino_LSM6DSOX.h>
#include <WiFiNINA.h>

void setup() {

  pinMode(LEDB, OUTPUT);
  pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);

  if (!IMU.begin()) {

    Serial.println("Failed to initialize IMU!");

    while (1);

  }


}

void loop() {
  if (IMU.temperatureAvailable())

  {

    int temperature_deg = 0;

    IMU.readTemperature(temperature_deg);


    Serial.print("LSM6DSOX Temperature = ");

    Serial.print(temperature_deg-3);

    Serial.println(" °C");

    temperature_deg=temperature_deg-3;

    if(temperature_deg>=32){
      digitalWrite(LEDR,HIGH);
      digitalWrite(LEDG,LOW);
      digitalWrite(LEDB,LOW);
    }
        
    else
      if(temperature_deg>20 && temperature_deg<32){
         digitalWrite(LEDR,LOW);
          digitalWrite(LEDG,HIGH);
          digitalWrite(LEDB,LOW);
      }
       
    else{
      digitalWrite(LEDR,LOW);
      digitalWrite(LEDG,LOW);
      digitalWrite(LEDB,HIGH);
    }
        

  }

}
