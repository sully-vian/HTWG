#include <WiFiNINA.h>

#define BUTTON_PIN 2 // see circuit in subject.pdf

int lastButtonState = LOW;

void setup() {
    Serial.begin(9600);
    pinMode(LEDB, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
}

void loop() {

    // read
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

    // write
    int currentButtonState = digitalRead(BUTTON_PIN);
    if (currentButtonState != lastButtonState) { // only send when update
        lastButtonState = currentButtonState;
        Serial.print(currentButtonState == HIGH ? '1' : '0');
    }

    delay(100);
}
