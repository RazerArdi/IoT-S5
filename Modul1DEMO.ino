const int ldrPin = 32;
const int buzzerPin = 18;

void setup() {
    Serial.begin(115200);
    pinMode(buzzerPin, OUTPUT);
}

void loop() {
    int ldrValue = analogRead(ldrPin);
    Serial.print("Nilai LDR: ");
    Serial.println(ldrValue);
    int frequency = map(ldrValue, 100, 4095, 100, 2000);

    if (ldrValue > 100) {
        tone(buzzerPin, frequency);
        Serial.print("Frekuensi Buzzer: ");
        Serial.println(frequency);
    } else {
        noTone(buzzerPin);
        Serial.println("Gelap! Buzzer Mati.");
    }

    delay(500);
}
