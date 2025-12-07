#include <WiFiNINA.h>

#define BUTTON_PIN 2 // see circuit in subject.pdf

int lastButtonState = LOW; // start with led off

void setup() {
    Serial.begin(9600);
    // prepare pins to use
    pinMode(LEDB, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
}

void loop() {

    // read what Node-RED sent
    if (Serial.available() > 0) {
        switch (Serial.read()) {
            case '1': // turn on light
                digitalWrite(LEDB, HIGH);
                break;
            case '0': // turn off
                digitalWrite(LEDB, LOW);
                break;
        }
    }

    // write to Node-RED
    int currentButtonState = digitalRead(BUTTON_PIN);
    if (currentButtonState != lastButtonState) { // only send when update
        lastButtonState = currentButtonState;
        Serial.print(currentButtonState == HIGH ? '1' : '0');
    }

    delay(100); // wait to avoid overloading Node-RED
}
