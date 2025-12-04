#include <WiFiNINA.h>
/*
 * This code makes the RGB led go from red to green to blue every half second.
 */

// the setup function runs once when you press reset or power the board
void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LEDB, OUTPUT);
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
    digitalWrite(LEDR, HIGH); // turn red on
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
    delay(500);

    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH); // turn green on
    digitalWrite(LEDB, LOW);
    delay(500);

    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, HIGH); // turn blue on
    delay(500);
}
