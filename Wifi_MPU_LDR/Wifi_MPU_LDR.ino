#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Pin untuk LDR
#define LDR_PIN 34

// Objek untuk MPU6050
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi sensor MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 tidak terdeteksi. Periksa koneksi!");
    while (1);
  }
  Serial.println("MPU6050 terdeteksi.");
  
  // Konfigurasi MPU6050
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  // Inisialisasi pin LDR
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  // Baca data LDR
  int ldrValue = analogRead(LDR_PIN);

  // Interpretasi data LDR
  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print(" - ");
  if (ldrValue < 10) {
  Serial.println("LDR tidak terhubung atau noise.");
  } else if(ldrValue < 50) {
    Serial.println("Keadaan gelap");
  } else if (ldrValue < 500) {
    Serial.println("Keadaan redup");
  } else if (ldrValue < 2000) {
    Serial.println("Cahaya terang");
  } else {
    Serial.println("Sangat terang");
  }
  
  // Baca data accelerometer dari MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Interpretasi kemiringan berdasarkan sumbu X dan Y
  float ax = a.acceleration.x;
  float ay = a.acceleration.y;

  Serial.print("Accelerometer X: "); Serial.println(a.acceleration.x);
  Serial.print("Accelerometer Y: "); Serial.println(a.acceleration.y);
  Serial.print("Accelerometer Z: "); Serial.println(a.acceleration.z);


  Serial.print("Kemiringan: ");
  if (ax > 10) {
  Serial.println("Kiri");
  } else if (ax < -10) {
  Serial.println("Kanan");
  } else if (ay > 10) {
  Serial.println("Depan");
  } else if (ay < -10) {
  Serial.println("Belakang");
  }


  // Delay untuk stabilitas
  delay(500);
}
