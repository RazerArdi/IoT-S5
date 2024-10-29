#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
int buzzerPin = 16;

// Batas sudut (derajat) untuk aktivasi buzzer
const int thresholdAngle = 30;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Inisialisasi MPU6050
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU6050 terkoneksi!");
  } else {
    Serial.println("Gagal menghubungkan ke MPU6050.");
  }
  
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
    
    // Menghitung sudut dari akselerometer
    float angleX = atan(ax / sqrt(ay * ay + az * az)) * 180 / PI;
    float angleY = atan(ay / sqrt(ax * ax + az * az)) * 180 / PI;

    Serial.print("Angle X: "); Serial.print(angleX);
    Serial.print("\tAngle Y: "); Serial.println(angleY);

    if (abs(angleX) > thresholdAngle) {
      if (angleX > 0) {
        Serial.println("Posisi: Belakang");
        tone(buzzerPin, 1000); // Bunyi 1 kHz
      } else {
        Serial.println("Posisi: Depan");
        tone(buzzerPin, 1500); // Bunyi 1.5 kHz
      }
    } else if (abs(angleY) > thresholdAngle) {
      if (angleY > 0) {
        Serial.println("Posisi: Kiri");
        tone(buzzerPin, 2000); // Bunyi 2 kHz
      } else {
        Serial.println("Posisi: Kanan");
        tone(buzzerPin, 2500); // Bunyi 2.5 kHz
      }
    } else {
      noTone(buzzerPin); // Matikan buzzer jika sudut dalam batas normal
    }

    delay(500); // Jeda waktu antar pembacaan sensor
  } else {
    Serial.println("Tidak ada koneksi WiFi, menunggu koneksi...");
    delay(1000);
  }
}
