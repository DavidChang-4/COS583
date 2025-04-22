// Pin assignments
const byte ENCODER1_A = 3;
const byte ENCODER1_B = 5;
const byte ENCODER2_A = 2;
const byte ENCODER2_B = 4;

// Variables for encoder count
volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

// Direction flags
volatile int dir1 = 0;
volatile int dir2 = 0;

// PPR and time settings
const int PPR = 600;  // Adjust based on your encoder spec
unsigned long lastTime = 0;
float rpm1 = 0;
float rpm2 = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(ENCODER1_A, INPUT);
  pinMode(ENCODER1_B, INPUT);
  pinMode(ENCODER2_A, INPUT);
  pinMode(ENCODER2_B, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(ENCODER1_A), encoder1ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER2_A), encoder2ISR, CHANGE);
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= 250) {  // update once per second
    noInterrupts();
    long count1 = encoder1Count;
    long count2 = encoder2Count;
    encoder1Count = 0;
    encoder2Count = 0;
    interrupts();

    rpm1 = (count1 / (float)PPR) * 60.0;
    rpm2 = (count2 / (float)PPR) * 60.0;

    Serial.print("1: ");
    // Serial.print((dir1 >= 0) ? "FWD " : "REV ");
    Serial.println(int(rpm1*4));
    // Serial.print(" RPM\t");

    Serial.print("2: ");
    // Serial.print((dir2 >= 0) ? "FWD " : "REV ");
    Serial.println(int(rpm2*4));
    // Serial.println(" RPM");

    lastTime = currentTime;
  }
}

// Interrupt service routines
void encoder1ISR() {
  bool A = digitalRead(ENCODER1_A);
  bool B = digitalRead(ENCODER1_B);
  dir1 = (A == B) ? 1 : -1;
  encoder1Count += dir1;
}

void encoder2ISR() {
  bool A = digitalRead(ENCODER2_A);
  bool B = digitalRead(ENCODER2_B);
  dir2 = (A == B) ? 1 : -1;
  encoder2Count += dir2;
}
