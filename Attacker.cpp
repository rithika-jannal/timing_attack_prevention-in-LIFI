// ESP32 ATTACKER CODE
// Timing Side-Channel Observer for LiFi Communication

#define LDR_PIN 34

int threshold = 300;      // Adjust if needed
int bitDuration = 300;    // Must match transmitter timing

// ---------- WAIT FOR SIGNAL ----------
void waitForStart() {

  while (true) {

    int val = analogRead(LDR_PIN);

    if (val > threshold) {
      delay(bitDuration);   // Sync with incoming signal
      break;
    }
  }
}

// ---------- READ SINGLE BIT ----------
int readBit() {

  // Sample at center of bit
  delay(bitDuration / 2);

  int val = analogRead(LDR_PIN);

  int bit = (val > threshold) ? 1 : 0;

  // Finish remaining duration
  delay(bitDuration / 2);

  return bit;
}

// ---------- READ 8 BITS ----------
void read8Bits() {

  Serial.print("Bits: ");

  for (int i = 0; i < 8; i++) {

    int bitVal = readBit();

    Serial.print(bitVal);
  }

  Serial.println();
}

// ---------- SETUP ----------
void setup() {

  Serial.begin(115200);

  Serial.println("ESP32 Timing Attack Observer");
}

// ---------- LOOP ----------
void loop() {

  waitForStart();

  read8Bits();

  delay(1000);
}
