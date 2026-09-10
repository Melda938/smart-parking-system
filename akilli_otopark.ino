#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD ekran ayarları ( genellikle 0x27 veya 0x3F )
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 18;
const int echoPin = 19;

// Ses hızı (cm/s cinsinden): 343 m/s = 34300 cm/s
#define SOUND_SPEED_CM_PER_S 34300.0

unsigned long durationMicroSec;
float durationSec;
float distanceCm;

// Park yeri eşik mesafesi 10 cm dir.
const float esikMesafesi = 10.0; 

void setup() {
  Serial.begin(115200);
  
  // Sensör pin ayarları:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // LCD ekranı başlatır
  lcd.init();                      
  lcd.backlight(); // Ekranın arka ışığını açar
}

void loop() {
  // Trigger pinini tetikler
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Echo pininden gelen süreyi mikrosaniye olarak okur
  durationMicroSec = pulseIn(echoPin, HIGH, 30000);
  
  // Mikrosaniyeyi açıkça saniyeye çevirir (1 saniye = 1.000.000 mikrosaniye)
  durationSec = (float)durationMicroSec / 1000000.0;
  
  // Mesafe hesabı: (Süre * Ses Hızı) / 2
  distanceCm = (durationSec * SOUND_SPEED_CM_PER_S) / 2.0;

  // --- LED EKRANA YAZDIRMA ---
  
  // 1. Satır: Anlık mesafeyi gösterir:
  lcd.setCursor(0, 0);
  lcd.print("Mesafe: ");
  lcd.print(distanceCm);
  lcd.print(" cm   ");
  // Eski karakterleri silmek için boşluk bırakılır.

  // 2. Satır: Dolu veya Boş durumu gösterir:
  lcd.setCursor(0, 1);
  if (distanceCm > 0 && distanceCm <= esikMesafesi) {
    lcd.print("Durum: YER DOLU ");
  } else {
    lcd.print("Durum: YER BOS  ");
  }

  delay(500);
}
