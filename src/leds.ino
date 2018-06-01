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

/* for witty board
void setled(String colour){
  analogWrite(red, 0);
  analogWrite(green, 0);
  analogWrite(blue, 0);
  if(colour == "red"){
   analogWrite(red, 255);
  }

  if(colour == "green"){
   analogWrite(green, 255);
  }

  if(colour == "blue"){
   analogWrite(blue, 255);
  }

  if(colour == "yellow"){
   analogWrite(red, 255);
   analogWrite(green, 255);
  }

  if(colour == "cyan"){
   analogWrite(green, 255);
   analogWrite(blue, 255);
  }

  if(colour == "magenta"){
   analogWrite(red, 255);
   analogWrite(blue, 255);
  }

  if(colour == "white"){
   analogWrite(red, 255);
   analogWrite(green, 255);
   analogWrite(blue, 255);
  }
} */
