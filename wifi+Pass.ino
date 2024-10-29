#include <WiFi.h>

const char* ssid = "Nama_SSID";
const char* password = "Password_SSID";

void setup() {
  Serial.begin(115200);
  connectWiFi();
}

void connectWiFi() {
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke jaringan WiFi...");
  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");

    // Jika setelah 10 detik masih belum terhubung, cari jaringan terbuka
    if (millis() - startAttemptTime >= 10000) {
      Serial.println("\nGagal menghubungkan ke jaringan, mencoba koneksi ke jaringan terbuka...");
      WiFi.disconnect();

      int n = WiFi.scanNetworks();
      for (int i = 0; i < n; ++i) {
        if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
          WiFi.begin(WiFi.SSID(i).c_str());
          while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
          }
          Serial.println("\nTerhubung ke jaringan terbuka: " + WiFi.SSID(i));
          return;
        }
      }
      Serial.println("Tidak ada jaringan terbuka yang tersedia.");
      return;
    }
  }
  Serial.println("\nTerhubung ke jaringan WiFi: " + String(ssid));
}

void loop() {
  // Program utama
}
