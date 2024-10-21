#define LED 2       // LED PIN
#define BUZZER 4    // BUZZER PIN

void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
}

void loop() {
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(BUZZER, HIGH);// turn the BUZZER on (HIGH is the voltage level)
    delay(1000);               // wait for a second

    digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(BUZZER, LOW); // turn the BUZZER off by making the voltage LOW
    delay(1000);               // wait for a second
}
