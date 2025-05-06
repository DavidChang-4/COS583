/*
  360° Potentiometer + Resistor Divider
  --------------------------------------------------------
  - Range: 2.5-5.0 V → analogRead: 512…1023
  - Dead-zone: 0 V → analogRead: 0 V
  - CW turn increments; CCW decrements
  - Crossing 5 V→2.5 V (wrap) also increments
*/

const int POT_PIN1     = A0;
const int POT_PIN2     = A2;
const int DEAD_THRESH  =   50; // readings < this are dead-zone
const int POT_MIN      =  512; // analogRead at 2.5 V
const int POT_MAX      = 1023; // analogRead at 5.0 V
const int NOISE_THRESH =    5; // ignore tiny jitters

long  counter1      = 0;
long  last_counter1 = 0;
int   lastValue1    = 0;
long  counter2      = 0;
long  last_counter2 = 0;
int   lastValue2    = 0;

const int button1Pin = 2;
bool button1Pressed = false;
const int button2Pin = 3;
bool button2Pressed = false;
const int button3Pin = 4;
bool button3Pressed = false;

void setup() {
  Serial.begin(115200);

  // wait until we get a stable reading in the valid range
  int rv;
  do {
    rv = analogRead(POT_PIN1);
  } while (rv < POT_MIN || rv < DEAD_THRESH);
  lastValue1 = rv;
  do {
    rv = analogRead(POT_PIN2);
  } while (rv < POT_MIN || rv < DEAD_THRESH);
  lastValue2 = rv;

  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button1Pin), onButton1Press, FALLING);
  attachInterrupt(digitalPinToInterrupt(button2Pin), onButton2Press, FALLING);
}

void onButton1Press() {
  button1Pressed = true;
}

void onButton2Press() {
  button2Pressed = true;
}

void loop() {
  int rv1 = analogRead(POT_PIN1);
  int rv2 = analogRead(POT_PIN2);

  // POTENTIOMETER 1
  // 1) ignore dead‐zone
  if (rv1 >= DEAD_THRESH) { // not in dead zone
    // 2) clamp into our 2.5–5 V window
    int rv = constrain(rv1, POT_MIN, POT_MAX);
    int delta = rv - lastValue1;

    // 3) wrap‐around CW?  (5 V→2.5 V)
    if ( lastValue1 > (POT_MAX - NOISE_THRESH) && rv < (POT_MIN + NOISE_THRESH) ) {
      counter1++;
      Serial.print("A:");
      Serial.println(counter1);
    }
    // 4) wrap‐around CCW? (2.5 V→5 V)
    else if ( lastValue1 < (POT_MIN + NOISE_THRESH) && rv > (POT_MAX - NOISE_THRESH) ) {
      counter1--;
      if (last_counter1 != counter1) {
        Serial.print("A:");
        Serial.println(counter1 - last_counter1);
      }
    }
    // 5) normal within‐range motion
    else if (abs(delta) > NOISE_THRESH) {
      if (delta > 0) {
        counter1++;
      } else {
        counter1--;
      }
      if (last_counter1 != counter1) {
        Serial.print("A:");
        Serial.println(counter1 - last_counter1);
      }
      // 6) store for next time
      lastValue1 = rv;
      last_counter1 = counter1;
    }
  }

  // POTENTIOMETER 2
  // 1) ignore dead‐zone
  if (rv2 >= DEAD_THRESH) { // not in dead zone
    // 2) clamp into our 2.5–5 V window
    int rv = constrain(rv2, POT_MIN, POT_MAX);
    int delta = rv - lastValue2;
    // 3) wrap‐around CW?  (5 V→2.5 V)
    if ( lastValue2 > (POT_MAX - NOISE_THRESH) && rv < (POT_MIN + NOISE_THRESH) ) {
      counter2++;
      Serial.print("B:");
      Serial.println(counter2);
    }
    // 4) wrap‐around CCW? (2.5 V→5 V)
    else if ( lastValue2 < (POT_MIN + NOISE_THRESH) && rv > (POT_MAX - NOISE_THRESH) ) {
      counter2--;
      if (last_counter2 != counter2) {
        Serial.print("B:");
        Serial.println(counter2 - last_counter2);
      }
    }
    // 5) normal within‐range motion
    else if (abs(delta) > NOISE_THRESH) {
      if (delta > 0) {
        counter2++;
      } else {
        counter2--;
      }
      if (last_counter2 != counter2) {
        Serial.print("B:");
        Serial.println(counter2 - last_counter2);
      }
      // 6) store for next time
      lastValue2 = rv;
      last_counter2 = counter2;
    }
  }

  delay(20);  // tweak for responsiveness vs. noise

  // BUTTON 1
  if (button1Pressed) {
    Serial.println("C");
    button1Pressed = false;  // Reset flag
  }

  // BUTTON 2
  if (button2Pressed) {
    Serial.println("D");
    button2Pressed = false;  // Reset flag
  }

  // BUTTON 3
  int button3State = digitalRead(button3Pin);
  if (button3State == LOW && !button3Pressed) {
    button3Pressed = true;
    Serial.println("E");
  }
  if (button3State == HIGH && button3Pressed) {
    button3Pressed = false; // Reset state when released
  }

}
