const int LED_PIN = 18;               // Pin digital yang terhubung ke LED
const int SWITCH_PIN = 17;            // Pin digital yang terhubung ke tombol switch

int switchState = 0;                  // Status tombol saat ini (HIGH atau LOW)
int lastSwitchState = 0;              // Status tombol sebelumnya untuk debouncing
unsigned long lastDebounceTime = 0;   // Waktu terakhir status tombol berubah
unsigned long debounceDelay = 50;      // Waktu untuk debouncing (mencegah noise dari switch)

bool systemUnlocked = false;          // Status sistem (apakah sistem terkunci atau tidak)
unsigned long unlockStartTime = 0;    // Waktu mulai untuk mengukur durasi penggunaan sistem
unsigned long unlockTimeout = 15000;   // Waktu (ms) untuk memasukkan kode sebelum timeout (15 detik)

// Variabel untuk deteksi pola input dari tombol
int buttonPressCount = 0;             // Menghitung jumlah tekanan tombol
unsigned long lastButtonPressTime = 0; // Waktu terakhir tombol ditekan
unsigned long buttonPressInterval = 15000;  // Batas waktu untuk pola input (15 detik)

unsigned long lastBlinkTime = 0;      // Waktu terakhir LED berkedip
int blinkInterval = 1000;             // Interval waktu kedipan LED (1 detik)
bool ledState = LOW;                  // Status nyala/mati LED (LOW = mati, HIGH = nyala)

// Variabel untuk fitur spam
bool spamEnabled = false;              // Status spam
unsigned long lastSpamBlinkTime = 0;   // Waktu terakhir LED berkedip saat spam

void setup() {
  Serial.begin(9600);                 // Menginisialisasi komunikasi serial pada baud rate 9600
  pinMode(SWITCH_PIN, INPUT);         // Mengatur SWITCH_PIN sebagai input
  pinMode(LED_PIN, OUTPUT);           // Mengatur LED_PIN sebagai output
  digitalWrite(LED_PIN, LOW);         // Memastikan LED dalam keadaan mati saat program dimulai
  Serial.println("Program dimulai. Sistem dalam keadaan terkunci."); // Menampilkan pesan di serial monitor
}

void loop() {
  // Membaca status tombol dengan debouncing
  int reading = digitalRead(SWITCH_PIN); // Membaca status tombol (HIGH jika ditekan, LOW jika tidak)

  // Debouncing untuk mencegah pembacaan berulang ketika tombol bergetar
  if (reading != lastSwitchState) {     // Jika status tombol berubah
    lastDebounceTime = millis();        // Catat waktu perubahan status
  }

  // Jika waktu debouncing telah berlalu
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Jika status tombol berubah setelah debouncing, simpan status tombol saat ini
    if (reading != switchState) {
      switchState = reading;            // Update status tombol

      // Jika tombol ditekan (dari LOW ke HIGH)
      if (switchState == HIGH) {
        buttonPressCount++;              // Tambah jumlah tekanan tombol
        lastButtonPressTime = millis();  // Simpan waktu saat tombol ditekan
        Serial.print("Tombol ditekan, jumlah tekan: "); // Tampilkan jumlah tekan
        Serial.println(buttonPressCount);

        // Jika sistem terkunci, periksa apakah pola input sesuai untuk membuka kunci
        if (!systemUnlocked && buttonPressCount == 2 && (millis() - lastButtonPressTime) < buttonPressInterval) {
          systemUnlocked = true;
          buttonPressCount = 0;               
          unlockStartTime = millis();     // Catat waktu ketika sistem dibuka
          Serial.println("Sistem terbuka! LED siap dikendalikan."); // Tampilkan status sistem
        }

        // Jika spam diaktifkan, matikan spam
        if (spamEnabled) {
          spamEnabled = false;            // Matikan spam
          digitalWrite(LED_PIN, LOW);     // Pastikan LED mati
          Serial.println("Spam dihentikan.");
        } 
        // Jika tombol ditekan 5 kali, aktifkan spam
        else if (buttonPressCount == 5 && (millis() - lastButtonPressTime) < buttonPressInterval) {
          spamEnabled = true;
          buttonPressCount = 0;                 
          Serial.println("Spam diaktifkan! LED berkedip setiap detik.");
        }
      }
    }
  }

  // Reset jika waktu untuk pola input habis atau salah
  if (!systemUnlocked && (millis() - lastButtonPressTime > buttonPressInterval)) {
    buttonPressCount = 0;               // Reset jumlah tekanan tombol jika waktu pola input habis
  }

  // Jika sistem terbuka, aktifkan logika kontrol LED
  if (systemUnlocked) {
    if (switchState == HIGH) {          // Jika tombol sedang ditekan
      unsigned long currentMillis = millis(); // Ambil waktu saat ini

      // Jika tombol ditekan, LED berkedip setiap 1 detik
      if (currentMillis - lastBlinkTime >= blinkInterval) {
        lastBlinkTime = currentMillis;   // Update waktu terakhir LED berkedip
        ledState = !ledState;            // Toggle status LED (nyala/mati)
        digitalWrite(LED_PIN, ledState); // Ubah status LED
      }
    } else {
      // Ketika tombol dilepas, matikan LED
      ledState = LOW;                    // Set status LED ke mati
      digitalWrite(LED_PIN, ledState);   // Ubah status LED
    }

    // Jika waktu untuk penggunaan sistem sudah habis, kunci kembali
    if (millis() - unlockStartTime >= unlockTimeout) {
      systemUnlocked = false;            // Kunci sistem kembali
      buttonPressCount = 0;              // Reset jumlah tekanan tombol
      Serial.println("Timeout! Sistem terkunci kembali."); // Tampilkan pesan timeout
    }
  }

  // Jika spam diaktifkan, LED berkedip setiap detik
  if (spamEnabled) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastSpamBlinkTime >= blinkInterval) {
      lastSpamBlinkTime = currentMillis; // Update waktu terakhir LED berkedip
      ledState = !ledState;               // Toggle status LED (nyala/mati)
      digitalWrite(LED_PIN, ledState);    // Ubah status LED
    }
  }

  lastSwitchState = reading;             // Update status terakhir tombol
}
