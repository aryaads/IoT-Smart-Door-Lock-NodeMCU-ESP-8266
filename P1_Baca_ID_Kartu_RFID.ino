#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D2  //D2
#define RST_PIN D1 //D1
#define BUZZER D3 //buzzer pin D3

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte i;
String ID_TAG;
//============
void readRFID(byte *buffer, byte bufferSize)
{
  ID_TAG="";
  for(byte i = 0;i<bufferSize; i++)
  {
    ID_TAG=ID_TAG+String(buffer[i], HEX);
  }
}
//=====================
void setup() { 
  Serial.begin(115200);
  pinMode(BUZZER,OUTPUT);
  pinMode(D4,OUTPUT);//relay
  pinMode(D0,OUTPUT);
  pinMode(D8,OUTPUT);
  pinMode(3,OUTPUT);
  digitalWrite(D4,HIGH);
  digitalWrite(D0,LOW);
  digitalWrite(D8,LOW);
  digitalWrite(3,LOW);
  delay(200); 
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Baca RFID Card");
  Serial.println("Tap kartu RFID ...");
}

//=============
void loop() {     
  if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()){
    return;
  }
  readRFID(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.print("ID : ");
  Serial.println(ID_TAG);  
  digitalWrite(BUZZER,HIGH);
  delay(500);
  digitalWrite(BUZZER,LOW);
  Serial.println("Tap kartu RFID ...");
  delay(2000);  
}
