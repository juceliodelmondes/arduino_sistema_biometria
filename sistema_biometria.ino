#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(7, 8);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;
String command;

void setup()
{
  Serial.begin(9600);
  finger.begin(57600);
}

void loop()                     // run over and over again
{
  //Serial.println("Ready to enroll a fingerprint!");
  //Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  if(Serial.available() > 0) {
    char c = Serial.read();
    if(c != "\n") command.concat(c);
    delay(20);
  } else if(!Serial.available()){
    if(command != "") {
      runCommand(command);
      command = "";
    }
  }
}

void runCommand(String command) {
  String param1 = command.substring(0,1); //comando w(write) ou d(delete)
  String param2 = command.substring(2); //id alvo
  if(param1 == "w" && param2 != "") {
    //vai gravar novo registro com o id do parametro 2
    id = param2.toInt();
    cadastrar();
  }
}

uint8_t cadastrar() {

  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:

      break;
    case FINGERPRINT_NOFINGER:
      Serial.print("Waiting ");
      Serial.println(id);
      delay(100);
      break;
    default:
      Serial.println("Error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:

      break;
    default:
      Serial.println("Error");
      return p;
  }

  Serial.print("Removef ");
  Serial.println(id);
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:

      break;
    case FINGERPRINT_NOFINGER:
      Serial.print("Waiting2 ");
      Serial.println(id);
      delay(100);
      break;
    default:
      Serial.println("Error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:

      break;
    default:
      Serial.println("Error");
      return p;
  }


  p = finger.createModel();
  if (p == FINGERPRINT_OK) {

  } else {
    Serial.println("Error");
    return p;
  }
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.print("Recorded ");
    Serial.println(id);
  } else {
    Serial.println("Error");
    return p;
  }

  return true;
}
