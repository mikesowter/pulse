void flashLEDs() {
  pinMode(BLU, OUTPUT);
  pinMode(GRN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(LDR, INPUT);
  setBlue();
  delay(300);
  setGreen();
  delay(300);
  setRed();
  delay(300);
  allOff();
}

void allOff() {
  digitalWrite(BLU,1);      //turn on Blue
  digitalWrite(GRN,1);      //turn off Green
  digitalWrite(RED,1);      //turn off Red
}

void setBlue() {
  digitalWrite(BLU,0);      //turn on Blue
  digitalWrite(GRN,1);      //turn off Green
  digitalWrite(RED,1);      //turn off Red
}

void setGreen() {
  digitalWrite(BLU,1);      //turn off Blue
  digitalWrite(GRN,0);      //turn on Green
  digitalWrite(RED,1);      //turn off Red
}

void setRed() {
  digitalWrite(BLU,1);      //turn off Blue
  digitalWrite(GRN,1);      //turn off Green
  digitalWrite(RED,0);      //turn on Red
}

void setWhite() {
  digitalWrite(BLU,0);      //turn on Blue
  digitalWrite(GRN,0);      //turn on Green
  digitalWrite(RED,0);      //turn on Red
}
