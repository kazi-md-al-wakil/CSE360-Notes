#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  //Change the HEX address
#include <Servo.h> 

Servo myservo1;

#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 10


int IR1 = 8;
int IR2 = 9;

int Slot = 4;           //Enter Total number of parking Slots

int flag1 = 0;
int flag2 = 0;

void setup() {

  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  myservo1.attach(7);
  myservo1.write(100);
  
  Serial.println("fingertest");
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  finger.begin(57600);

  
   if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); 
  Serial.print(finger.templateCount); 
  Serial.println(" templates");
  Serial.println("Waiting for valid finger...");

  
  lcd.init();
  lcd.backlight();
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  


  lcd.setCursor (0,0);
  lcd.print("   ARDUINO BASED   ");
  lcd.setCursor (0,1);
  lcd.print(" PARKING SYSTEM ");
  delay (2000);
  lcd.clear();  
}



void loop(){ 
  getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  
  DHT.read(DHT11_PIN);
  
 if(digitalRead (IR1) == LOW && flag1==0){
 if(Slot>0){flag1=1;
 if(flag2==0){myservo1.write(0); Slot = Slot-1;}
 }else{
lcd.setCursor (0,0);
lcd.print("    SORRY :(    ");  
lcd.setCursor (0,1);
lcd.print("  Parking Full  "); 
delay (3000);
lcd.clear(); 
}
}

if(digitalRead (IR2) == LOW && flag2==0){flag2=1;
if(flag1==0){myservo1.write(0); Slot = Slot+1;}
}

if(flag1==1 && flag2==1){
delay (1000);
myservo1.write(100);
flag1=0, flag2=0;
}

lcd.setCursor (0,0);
lcd.print("Tem:");
lcd.setCursor (4,0);
lcd.print(DHT.temperature);
lcd.setCursor (6,0);
lcd.print("'C");
lcd.setCursor (9,0);
lcd.print("Hum:");
lcd.setCursor (13,0);
lcd.print(DHT.humidity);
lcd.setCursor (15,0);
lcd.print("%");
lcd.setCursor (0,1);
lcd.print("Slot Left: ");
lcd.print(Slot);
}


uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
       digitalWrite(11, HIGH);
       delay(2500);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    digitalWrite(11, HIGH);
    delay(2500);
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
  {digitalWrite(11, HIGH);
  delay(3000);
  digitalWrite(11, LOW);
  Serial.print("Not Found"); 
  Serial.print("Error"); 
  return finger.fingerID;
 }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #



int getFingerprintIDez() {
  
  uint8_t p = finger.getImage();
  
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  
  p = finger.fingerFastSearch();

  
  if (p != FINGERPRINT_OK)  return -1; 
  

  // found a match!

  {
    digitalWrite(12, HIGH);
    delay(3000);
    digitalWrite(12, LOW);
    Serial.print("Found ID #"); 
    Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); 
    Serial.println(finger.confidence);
  
  }
  
  }
