const int ledPin[] = {7, 8, 9, 10};
const int buttonPin = 4;

int buttonState = 0;
int programNo = 1;


void setup()
{
  for (int i =0;i<4;i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(buttonPin, INPUT_PULLUP);
  displayBinary(programNo);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  delay(300);
  //If button pressed...
  if (buttonState == LOW) {
    if (programNo == 15) {
      programNo = 1;
      displayBinary(programNo);
    } else { 
      programNo ++;
      displayBinary(programNo);
    }
  }
}

void displayBinary(byte numToShow) {
  for (int i =0;i<4;i++)
  {
    if (bitRead(numToShow, i)==1)
    {
      digitalWrite(ledPin[i], HIGH); 
    }
    else
    {
      digitalWrite(ledPin[i], LOW); 
    }
  }
}
