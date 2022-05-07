#include <LiquidCrystal.h>

const int greenLed = 2;
const int redLed = 3;
LiquidCrystal lcd(12, 13, 11, 10, 9, 8);
int val;
char receivedChar;
boolean newData = false;
void setup() {
  // put your setup code here, to run once:
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  Serial.println("<Arduino is ready>");
  /*lcd.begin(16, 2);
    lcd.print("Press A to park");
    lcd.setCursor(0,1);
    lcd.print("Press B to leave");*/


}

void loop() {
  recvOneChar();
  showNewData();
  /*// put your main code here, to run repeatedly:
    while (!Serial.available()){}
    //delay(100); //allows all serial sent to be received together
    byte b1 = Serial.read();
    while (!Serial.available()){}
    //  delay(100); //allows all serial sent to be received together
    byte b2 = Serial.read();

    val = b2  + b1 *256 ;
    val = Serial.parseInt();
    Serial.println(val);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Position:");
    lcd.print(val + 1);
    lcd.setCursor(10,0);
    lcd.print("   ");*/
}

void recvOneChar() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    newData = true;
  }
}

void showNewData() {
  if (newData == true) {
    Serial.print("This just in ... ");
    Serial.println(receivedChar);
    switch (receivedChar) {
      case 'd': // Press A to enter / Press B to leave
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Press A to enter");
        lcd.setCursor(0, 1);
        lcd.print("Press B to leave");
        break;
      case 'f': // System is full / Press B to leave
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("System is full");
        lcd.setCursor(0, 1);
        lcd.print("Press B to leave");
        break;
      case 'F': // Warning:No empty / spot in system!
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Warning:No empty");
        lcd.setCursor(0, 1);
        lcd.print("spot in system!");
        break;
      case 'G': // Turn led green on, red led off
        digitalWrite(greenLed, HIGH);
        digitalWrite(redLed, LOW);
        break;
      case 'R': // Turn led red on, green led off
        digitalWrite(greenLed, LOW);
        digitalWrite(redLed, HIGH);
        break;
      case 'p': // Insert the car / Press A after
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Insert the car");
        lcd.setCursor(0, 1);
        lcd.print("Press A after");
        break;
      case 'b': // Enter car number / to call your car
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter car number");
        lcd.setCursor(0, 1);
        lcd.print("to call your car");
        break;
      case 'e': // Please wait next/ spot is coming
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please wait next");
        lcd.setCursor(0, 1);
        lcd.print("spot is coming");
        break;
      case '1':// Your car number is 'char' / Press A to end
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your number is 1");
        lcd.setCursor(0, 1);
        lcd.print("Press A to end");
        break;
      case '2':
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your number is 2");
        lcd.setCursor(0, 1);
        lcd.print("Press A to end");      
        break;
      case '3':
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your number is 3");
        lcd.setCursor(0, 1);
        lcd.print("Press A to end");
        break;
      case '4':
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your number is 4");
        lcd.setCursor(0, 1);
        lcd.print("Press A to end");
        break;
      case '5':
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your number is 5");
        lcd.setCursor(0, 1);
        lcd.print("Press A to end");
        break;
      case '6':
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your number is 6");
        lcd.setCursor(0, 1);
        lcd.print("Press A to end");
        break;
      case 'h': // Thank you! / Have a good day!
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Thank you!");
        lcd.setCursor(0, 1);
        lcd.print("Have a good day!");
        break;
      case 's': // Thank you / See you soon
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Thank you");
        lcd.setCursor(0, 1);
        lcd.print("See you soon");
        break;
      case 'w': // Please wait, the / car will arrive
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please wait, the");
        lcd.setCursor(0, 1);
        lcd.print("car will arrive");
        break;
      case 'y': // Your car is here / Press B to end
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your car is here");
        lcd.setCursor(0, 1);
        lcd.print("Press B to end");  
        break;
      case 'n': // Warning: This // spot is empty      
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Warning:This");
        lcd.setCursor(0, 1);
        lcd.print("spot is empty!");  
        break;  
    }
    newData = false;
  }
}
