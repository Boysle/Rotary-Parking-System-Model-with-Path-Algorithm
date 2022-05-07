#include <Keypad.h>
#include <Servo.h>



int Pin1 = 10;
int Pin2 = 11;
int Pin3 = 12;
int Pin4 = 13;
int sensorPin = 2;
int servoPin = 3;



const byte satir = 2;
const byte sutun = 4;

char tus;
char tus_takimi[satir][sutun] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'}
};

byte satir_pinleri[satir] = {9, 8};
byte sutun_pinleri[sutun] = {7, 6, 5, 4};

int temp = 0;
int sensor = 0;
int _step = 0;
int wait = 0;
int pointer = 0;
int pointer_temp = 0;
int rotation_num = 0;
int mycar = 0;
int exit_rot_num = 0;
int data[6] = {0, 0, 0, 0, 0, 0};
boolean dir = false;
boolean harf_key = true;
boolean num_key = true;
boolean servo_key = true;
boolean enter = true;
boolean full = false;
boolean error = false;

Keypad keypadd = Keypad(makeKeymap(tus_takimi), satir_pinleri, sutun_pinleri, satir, sutun);
Servo Servo1;

void setup() {
  Serial.begin(9600);
  Servo1.attach(servoPin);
  //Serial.println("START");
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(Pin4, OUTPUT);
}

void loop() {
  Servo1.write(81);
  delay(1000);
  full = checkFull(data);
  if (!full) {
    Serial.write('d'); //d means default state, instructions will be displayed
    Serial.write('G'); //G means green led is on, Red led is off
  }
  else {
    Serial.write('f'); //f means full state, will replace A's line
    Serial.write('R'); //R means red led is on, Green led is off
  }
  while (harf_key) {
    tus = keypadd.getKey();
    if (tus == 'A') {
      full = checkFull(data);
      if (full) {
        //Serial.println("The system is full!");
        Serial.write('F'); //F means warning full state, will be shown for 3 seconds
        error = true;
        delay(3000);
        Serial.write('f'); //turn back to f state
      }
      else {
        enter = true;
        Serial.write('p'); //p means insert car, press A to go
        /*Serial.write(pointer / 256);
          Serial.write(pointer % 256);
          delay(50);
          Serial.print("Pointer: ");
          Serial.println(pointer);*/
        harf_key = false;
        //Serial.println(tus);
      }
      tus = 'a';
    }
    else if (tus == 'B') {
      Serial.write('b'); //b means "B" state, asking the user to press a number
      enter = false;
      harf_key = false;
      //Serial.println(tus);
    }
    delay(80);
  }


  while (num_key && !enter) {
    tus = keypadd.getKey();
    if (tus == '1') {
      mycar = 0;
      num_key = false;
    }
    else if (tus == '2') {
      mycar = 1;
      num_key = false;
    }
    else if (tus == '3') {
      mycar = 2;
      num_key = false;
    }
    else if (tus == '4') {
      mycar = 3;
      num_key = false;
    }
    else if (tus == '5') {
      mycar = 4;
      num_key = false;
    }
    else if (tus == '6') {
      mycar = 5;
      num_key = false;
    }
    delay(80);
  }
  //Serial.println(tus);
  harf_key = true;
  num_key = true;

  if (enter) { //for entering the car
    Servo1.write(10);
    delay(1000);
    while (harf_key) { //input for saying close the bar
      tus = keypadd.getKey();
      if (tus == 'A') {
        harf_key = false;
        tus = 'a';
      }
      delay(80);
    }
    harf_key = true;
    Servo1.write(81);
    writePos(pointer); //this is used to show user's number, will give 1,2,3,4,5,6 chars, press A to end
    while (harf_key) {
      tus = keypadd.getKey();
      if (tus == 'A') {
        harf_key = false;
        tus = 'a';
      }
    }
    harf_key = true;
    Serial.write('s'); //say thank you, your car will be safe here, for 3 seconds
    delay(3000);
    data[pointer] = 1;
    full = checkFull(data);
    if (full) {
      Serial.write('R'); //R means red led is on, Green led is off
    }
    else {
      Serial.write('e'); //e means please wait while the next empty spot is arriving
    }
    rotation_num = rot_num_finder(pointer, data);
    Rotate(rotation_num);
    pointer_temp = pointer + rotation_num;
    if (pointer_temp >= 0) {
      pointer = pointer_temp % 6;
    }
    if (pointer_temp < 0) {
      pointer = 6 + pointer_temp ;
    }
    printData(data, pointer);
  }
  else { //for exiting the car
    if (data[mycar] == 1) {
      exit_rot_num = mycar - pointer;
      if ((exit_rot_num) < -3) {
        exit_rot_num = 6 + exit_rot_num;
      }
      if ((exit_rot_num) > 3) {
        exit_rot_num = -6 + exit_rot_num;
      }
      Serial.write('w'); //w means please wait for the car to arrive
      Rotate(exit_rot_num);
      pointer = mycar;
      data[pointer] = 0;
      printData(data, pointer);
      Serial.write('y'); //y means your car is here, press B to end
      Servo1.write(10);
      while (harf_key) { //input for saying "i am done"
        tus = keypadd.getKey();
        if (tus == 'B') {
          harf_key = false;
          tus = 'b';
        }
        delay(80);
      }
      harf_key = true;
      Servo1.write(81);
      Serial.write('h'); //say have a good day, will last for 3 seconds
      delay(3000);
    }
    else {
      //Serial.println("There is no car in this spot!");
      Serial.write('n'); // n means there is no car in the spot, wait for 3 seconds
      delay(3000);
      error = true;
    }
  }
  error = false;
}

void Rotate(int x) {
  int i = 0;
  int _step = 0;
  int sensor = digitalRead(sensorPin);
  if (x < 0) {
    dir = true;
    x *= (-1);
  } else {
    dir = false;
  }
  while (i < x) {
    wait = 0;
    while (wait <= 1000) {
      switch (_step) {
        case 0:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, HIGH);
          break;
        case 1:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, HIGH);
          break;
        case 2:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, LOW);
          break;
        case 3:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, LOW);
          break;
        case 4:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 5:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 6:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 7:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, HIGH);
          break;
        default:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
      }
      if (dir) {
        _step++;
      } else {
        _step--;
      }
      if (_step > 7) {
        _step = 0;
      }
      if (_step < 0) {
        _step = 7;
      }
      delay(2);
      wait++;
    }
    //////////////////////////////////////////////////////
    sensor = 1;
    while (sensor == 1) {
      if (temp >= 15) {
        temp = 0;
        sensor = digitalRead(sensorPin);
        if (sensor == 0) {
          i++;
          //Serial.print("Our i value: ");
          //Serial.println(i);
        }
      }
      switch (_step) {
        case 0:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, HIGH);
          break;
        case 1:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, HIGH);
          break;
        case 2:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, LOW);
          break;
        case 3:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, LOW);
          break;
        case 4:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 5:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 6:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 7:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, HIGH);
          break;
        default:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
      }
      if (dir) {
        _step++;
      } else {
        _step--;
      }
      if (_step > 7) {
        _step = 0;
      }
      if (_step < 0) {
        _step = 7;
      }
      delay(2);
      temp ++;
    }
  }
  /////////////////////////////////////////////////////////
  wait = 0;
  if (!(x == 0)) {
    int waitmark;
    if(!dir){
      waitmark = 120;
    }
    else{
      waitmark = 250;
    }
    while (wait <= waitmark) {
      switch (_step) {
        case 0:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, HIGH);
          break;
        case 1:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, HIGH);
          break;
        case 2:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, LOW);
          break;
        case 3:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, LOW);
          break;
        case 4:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 5:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 6:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 7:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, HIGH);
          break;
        default:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
      }
      if (dir) {
        _step++;
      } else {
        _step--;
      }
      if (_step > 7) {
        _step = 0;
      }
      if (_step < 0) {
        _step = 7;
      }
      delay(2);
      wait++;
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
    }
  }
}

int rot_num_finder(int p, int d[]) {
  int i = 1;
  int j = 0;
  int result;
  bool isfull = true;

  while (1) { //infinite loop
    if (d[(p + i) % 6] == 0) { // going to right, positive, use mod(6)
      result = i;
      break;
    }
    if (p - i >= 0) { // check if pointer would be negative
      if (d[p - i] == 0) {
        result = -i;
        break;
      }
    }
    if ( p - i < 0) { // if negative, use 6-(p-i) instead
      if (d[6 + (p - i)] == 0) {
        result = -i;
        break;
      }
    }
    i++;
  }
  while (j < 6) {
    if (data[j] == 0) {
      isfull = false;
    }
    j++;
  }
  if (isfull) {
    result = 0;
  }

  return result;
}

void printData(int data[], int pointer) {
  int j = 0;
  while (j < 6) {
    //Serial.print(data[j]);
    //Serial.print("  ");
    j++;
  }
  //Serial.print("\nOur new location:");
  //Serial.print(pointer);
}

bool checkFull(int data[]) {
  int j = 0;
  bool isFull = true;
  while (j < 6) {
    if (data[j] == 0) {
      isFull = false;
    }
    j++;
  }
  return isFull;
}

void writePos(int p) {
  switch (p) {
    case 0:
      Serial.write('1');
      break;
    case 1:
      Serial.write('2');
      break;
    case 2:
      Serial.write('3');
      break;
    case 3:
      Serial.write('4');
      break;
    case 4:
      Serial.write('5');
      break;
    case 5:
      Serial.write('6');
      break;
  }
}
