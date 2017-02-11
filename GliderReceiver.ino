#include <Servo.h>
#include <SoftwareSerial.h>

Servo myservo1,myservo2;

int servoPin1 = 3;
int servoPin2 = 4;

int posX = 90;
int posY = 90;

SoftwareSerial radio(6, 7); // RX, TX

long timeToRead = 40;
long oldTimeRead = 0;


void setup() {
  // put your setup code here, to run once:
  myservo1.attach(servoPin1);
  myservo2.attach(servoPin2);

  radio.begin(9600);
  Serial.begin(115200);

  myservo1.write(90);
  myservo2.write(90);

  oldTimeRead = millis();

}

String data = "";
String command = "";

void loop() {
  // put your main code here, to run repeatedly:
  if (radio.available()>0){
    readData();
  }

  if (millis() - oldTimeRead > timeToRead){
      myservo1.write(posX);
      myservo2.write(posY);
  }
  
}

void readData(){
  while (radio.available()>0){
      char c = radio.read();      
      data += c;
      if (c == '}'){
        readCommand();
        getParams();
      }
  }
}

void readCommand(){
  int commandEnd = data.lastIndexOf('}');
  String dataTrim = data.substring(0,commandEnd+1);
  int commandStart = dataTrim.lastIndexOf('{');
  if (commandEnd - commandStart >= 5){
    command = dataTrim.substring(commandStart,commandEnd+1);  
  }
  data = data.substring(commandEnd+1);
}

void getParams(){
  int commaPos = command.indexOf(',');
  String strX = command.substring(1,commaPos);
  String strY = command.substring(commaPos+1,command.length()-1);
  posX = strX.toInt();
  posY = strY.toInt();
//  Serial.print("X: ");
//  Serial.print(strX);
//  Serial.print(" Y: ");
//  Serial.print(strY);
//  Serial.println();
}

