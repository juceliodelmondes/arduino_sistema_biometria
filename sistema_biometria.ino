#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(7, 8);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;
String command;
int timer = 0;

void setup()
{
  Serial.begin(9600);
  finger.begin(57600);
  pinMode(3, OUTPUT);
  tone(3, 330);
  delay(200);
  tone(3, 392, 200);
}

void loop()
{
  if(timer == 50) {
    Serial.println("? ?");
    timer = 0;
  }
  if(Serial.available() > 0) {
    char c = Serial.read();
    if(c != "\n") command.concat(c);
    delay(20);
  } else {
    if(command != "") {
      runCommand(command);
      command = "";
    } else {
      realizarLeitura();
    }
  }
  delay(10);
  timer++;
}

void runCommand(String command) {
  String param1 = command.substring(0,1); //comando w(write) ou d(delete)
  String param2 = command.substring(2); //id alvo
  if(param1 == "w" && param2 != "") {
    //vai gravar novo registro com o id do parametro 2
    id = param2.toInt();
    cadastrar();
  }

  if(param1 == "d" && param2 != "") { //deletar determinado cadastro
    finger.deleteModel(param2.toInt());
    Serial.println(command);
  }
}

void cadastrar() {
  int limiteLoop = 100;
  int contadorLoop = 0;
  int p = -1;
  while (p != FINGERPRINT_OK && contadorLoop < limiteLoop) {
    contadorLoop++;
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
      Serial.print("Error ");
      Serial.println(id);
      break;
    }
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:

      break;
    default:
      Serial.print("Error ");
      Serial.println(id);
      return p;
  }

  Serial.print("Removef ");
  Serial.println(id);
  finger.LEDcontrol(false);
  delay(2000);
  finger.LEDcontrol(true);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  contadorLoop = 0;
  while (p != FINGERPRINT_OK && contadorLoop < limiteLoop) {
    contadorLoop++;
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
      Serial.print("Error ");
      Serial.println(id);
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:

      break;
    default:
      Serial.print("Error ");
      Serial.println(id);
      return p;
  }


  p = finger.createModel();
  if (p == FINGERPRINT_OK) {

  } else {
    Serial.print("Error ");
    Serial.println(id);
    return p;
  }
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.print("Recorded ");
    Serial.println(id);
    tone(3, 392, 500);
    for(int i = 0; i < 12; i++){
     if(i % 2 == 0) finger.LEDcontrol(false);
     if(i % 2 != 0) finger.LEDcontrol(true);
     delay(300);
    }
    noTone(3);
  } else {
    Serial.print("Error ");
    Serial.println(id);
    return p;
  }
}
//==============================================
void realizarLeitura() {
  if(finger.getImage() == FINGERPRINT_OK && finger.image2Tz() == FINGERPRINT_OK && finger.fingerSearch() ==  FINGERPRINT_OK) {
    for(int i = 0; i < 10; i++) {
      if(i % 2 == 0) finger.LEDcontrol(false);
      if(i % 2 != 0) finger.LEDcontrol(true);
      delay(10);
    }
    Serial.print("access ");
    Serial.print(finger.fingerID);
    Serial.print(" ");
    Serial.println(finger.confidence);    
  
    finger.LEDcontrol(false);
    delay(1000);
  }
}
