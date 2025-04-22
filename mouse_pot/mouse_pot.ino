const int vin1Pin = A0;

int increment = 3;
int prevVal = -1;

void setup() {
  Serial.begin(9600);  // Start serial communication
  delay(2000);
}

void loop() {
  // Read voltage
  int diff = 0;
  int vin1Read = analogRead(vin1Pin);

  if (vin1Read <480 || vin1Read > 1010) { // Skip if voltage outside range
      return;
  }
  int vin1Map = map(vin1Read, 480, 1010, 0, 100); // Remap
  
  if (prevVal == -1) {
    prevVal = vin1Map;
    return;
  }
  diff = vin1Map - prevVal;
  if (abs(vin1Map - prevVal) < increment) {
    return;
  }
  // if (abs(vin1Map - prevVal) > increment) {
  //   if (prevVal > 75 && vin1Map < 25) { // Looping from 100 forward to 0
  //     diff = (100-prevVal) + vin1Map;
  //   }
  //   else if (prevVal < 75 && vin1Map > 25) { // Looping from 0 back to 100
  //     diff = -(100-prevVal + vin1Map);
  //   }
  //   else {
  //     diff = vin1Map - prevVal;
  //   } 
  // }
  // if (diff != 0) {
  //   Serial.println(diff); // Serial send counter
  // }
  if (diff==3 or diff==-3) {
    Serial.println(diff);
  }

  prevVal = vin1Map;
}