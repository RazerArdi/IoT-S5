#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// WiFi Credentials
const char* ssid = "TEST";
const char* password = "12345678";

// Server URL
const char* serverUrl = "http://192.168.60.71/modul4/save_sensor_data.php";

// Pin untuk LDR
#define LDR_PIN 34

// Objek untuk MPU6050
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  // Inisialisasi WiFi
  Serial.print("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi terhubung!");

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

  // Baca data accelerometer dari MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Interpretasi kemiringan berdasarkan sumbu X dan Y
  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  String arah = "Netral";
  if (ax > 10) {
    arah = "Kiri";
  } else if (ax < -10) {
    arah = "Kanan";
  } else if (ay > 10) {
    arah = "Depan";
  } else if (ay < -10) {
    arah = "Belakang";
  }

  // Tambahkan log
  Serial.print("angka_sumbu: ");
  Serial.println(ax, 2);  // 2 angka desimal
  Serial.print("arah: ");
  Serial.println(arah);

  // Kirim data ke server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverUrl);  // Tentukan URL server
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  // Tambah header

    // Data yang akan dikirim via POST
    String postData = "angka_sumbu=" + String(ax, 2) + "&arah=" + arah;  // Tambahkan format desimal
    Serial.print("Data POST: ");
    Serial.println(postData);

    // Kirim POST request
    int httpResponseCode = http.POST(postData);

    // Cek respons server
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println("Server response: " + http.getString());
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Akhiri koneksi
  } else {
    Serial.println("WiFi terputus!");
  }

  delay(2000);
}
