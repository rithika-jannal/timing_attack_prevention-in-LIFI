
// FINAL WORKING CODE
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Change address if needed (0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define LED_PIN 13
#define LDR_PIN A0

int threshold = 500;
String message = "Hello";

// ---------- TIMINGS ----------
int bitDuration = 300;

int pulseOn  = 300;
int pulseOff = 300;
int bitGap   = 800;

int mode = 0;

// ---------- SETUP ----------
void setup() {

  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  lcd.init();
lcd.backlight();

lcd.setCursor(0, 0);
lcd.print("LiFi System");
lcd.setCursor(0, 1);
lcd.print("Starting...");
delay(1500);
lcd.clear();

  Serial.begin(9600);

  Serial.println("LiFi System Starting...");
}

// ---------- AUTO CALIBRATION ----------
void autoCalibrate() {
  Serial.println("Calibrating...");

  digitalWrite(LED_PIN, LOW);
  delay(500);
  int offVal = analogRead(LDR_PIN);

  digitalWrite(LED_PIN, HIGH);
  delay(500);
  int onVal = analogRead(LDR_PIN);

  digitalWrite(LED_PIN, LOW);

  threshold = (onVal + offVal) / 2;

  Serial.print("OFF: "); Serial.println(offVal);
  Serial.print("ON : "); Serial.println(onVal);
  Serial.print("Threshold: "); Serial.println(threshold);
}

// ---------- LDR SAMPLE (Plotter Compatible) ----------
int sampleLDR() {
  int val = analogRead(LDR_PIN);

  // Real-time visualization
  Serial.print(val);
  Serial.print(" ");
  Serial.println(threshold);

  return (val > threshold) ? 1 : 0;
}

// ---------- NORMAL MODE ----------
int sendReadBitNormal(int bitVal) {

  digitalWrite(LED_PIN, bitVal);
  delay(bitDuration / 2);

  int received = sampleLDR();

  delay(bitDuration / 2);
  digitalWrite(LED_PIN, LOW);
  delay(100);

  return received;
}

// ---------- SECURE MODE ----------
int sendReadBitSecure(int bitVal) {

  int pulses = (bitVal == 0) ? 1 : 2;
  int counted = 0;

  for (int i = 0; i < pulses; i++) {

    digitalWrite(LED_PIN, HIGH);
    delay(pulseOn / 2);

    counted += sampleLDR();

    delay(pulseOn / 2);

    digitalWrite(LED_PIN, LOW);
    delay(pulseOff);
  }

  delay(bitGap);

  Serial.print("[");
  Serial.print(counted);
  Serial.print("] ");

  return (counted >= 2) ? 1 : 0;
}

// ---------- CHAR ----------
char sendAndReceiveChar(char ch) {

  byte received = 0;

  for (int i = 7; i >= 0; i--) {

    int bitVal = bitRead(ch, i);
    int rx;

    if (mode == 1)
      rx = sendReadBitNormal(bitVal);
    else
      rx = sendReadBitSecure(bitVal);

    received |= (rx << i);
  }

  return (char)received;
}

// ---------- MESSAGE ----------
void sendMessage() {


  String output = "";
  unsigned long t0 = millis();

  for (int i = 0; i < message.length(); i++) {
    char r = sendAndReceiveChar(message[i]);
    output += r;
    delay(300);
  }

  unsigned long t1 = millis();

  Serial.println();
  Serial.print("Received: "); Serial.println(output);
  lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Received:");

lcd.setCursor(0, 1);
lcd.print(output);

  Serial.print("Time: ");
  Serial.print(t1 - t0);
  Serial.println(" ms");

  if (output == message)
    Serial.println("Match OK");
  else
    Serial.println("Error");

  if (mode == 1)
    Serial.println("Normal Mode (Vulnerable)");
  else
    Serial.println("Secure Mode (Protected)");

  Serial.println("-----------------------");
}

// ---------- LOOP ----------
void loop() {

  Serial.println("\nSelect Mode:");
  Serial.println("1 → Normal");
  Serial.println("2 → Secure");

  while (Serial.available() == 0);

  char input = Serial.read();

  while (Serial.available()) Serial.read();

  if (input == '1') {
    mode = 1;
    Serial.println("Normal Mode");
  }
  else if (input == '2') {
    mode = 2;
    Serial.println("Secure Mode");
  }
  else {
    Serial.println("Invalid input");
    return;
  }
  lcd.setCursor(12, 0);
if (mode == 1)
    lcd.print("N");
else
    lcd.print("S");
  delay(500);

  autoCalibrate();

  delay(500);

  sendMessage();

  delay(2000);
}