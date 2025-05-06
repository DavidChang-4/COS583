/*
  Motor Encoder
  --------------------------------------------------------
  - Read A and B square waves from motor encoders. 
*/

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
const int PPR = 300;
unsigned long lastTime = 0;
float rpm1 = 0;
float rpm2 = 0;

// Buttons
const int button1Pin = 6;
bool button1Pressed = false;
const int button2Pin = 7;
bool button2Pressed = false;
const int button3Pin = 8;
bool button3Pressed = false;

void setup() {
  Serial.begin(115200);
  
  // Encoders
  pinMode(ENCODER1_A, INPUT);
  pinMode(ENCODER1_B, INPUT);
  pinMode(ENCODER2_A, INPUT);
  pinMode(ENCODER2_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER1_A), encoder1ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER2_A), encoder2ISR, CHANGE);

  // Buttons
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= 100) {  // collect PWM signals
    noInterrupts();
    long count1 = encoder1Count;
    long count2 = encoder2Count;
    encoder1Count = 0;
    encoder2Count = 0;
    interrupts();

    rpm1 = (count1 / (float)PPR) * 60.0 * 4;
    rpm2 = (count2 / (float)PPR) * 60.0 * 4;

    Serial.print("B:");
    // Serial.print((dir1 >= 0) ? "FWD " : "REV ");
    Serial.println(int(rpm1));
    // Serial.print(" RPM\t");

    Serial.print("A:");
    // Serial.print((dir2 >= 0) ? "FWD " : "REV ");
    Serial.println(int(rpm2));
    // Serial.println(" RPM");

    lastTime = currentTime;
  }
  // BUTTON 1
  if (button1State == LOW && !button1Pressed) {
    button1Pressed = true;
    Serial.println("C");
  }
  if (button1State == HIGH && button1Pressed) {
    button1Pressed = false;
  }
  // BUTTON 2
  if (button2State == LOW && !button2Pressed) {
    button2Pressed = true;
    Serial.println("D");
  }
  if (button2State == HIGH && button2Pressed) {
    button2Pressed = false;
  }

  // BUTTON 3 
  if (button3State == LOW && !button3Pressed) {
    button3Pressed = true;
    Serial.println("E");
  }
  if (button3State == HIGH && button3Pressed) {
    button3Pressed = false;
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