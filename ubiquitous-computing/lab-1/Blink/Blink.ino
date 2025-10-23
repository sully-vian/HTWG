void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);
  Serial.print("heyy\n");
  digitalWrite(LED_BUILTIN,LOW);
  delay(1000);
}
