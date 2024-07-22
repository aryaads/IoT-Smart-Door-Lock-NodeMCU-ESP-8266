#define BLYNK_TEMPLATE_ID "TMPL6VDy05w3p"
#define BLYNK_TEMPLATE_NAME "IoT Smart RFID Door Lock"
#define BLYNK_AUTH_TOKEN "4N3TlhwQ2oXOhLS1lrOjxbFBrryf4w7Z"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = BLYNK_AUTH_TOKEN;;

char ssid[] = "DarkWeB";
char pass[] = "aryadws1928";

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D2  // sda pin D2
#define RST_PIN D1 // RST (flash) pin D2

#define relay D4 //Relay Pin D4
#define BUZZER D3 //buzzer pin D8
#define LED_R D0 //LED R (RGB)
#define LED_G 3  //LED G (RGB) (pin RX0)
#define LED_B D8  //LED B (RGB)

MFRC522 mfrc522(SS_PIN, RST_PIN);        

BlynkTimer timer;
WidgetTerminal terminal(V0);
byte i,j,k,f_cek;
String ID_TAG;
String ID[10];
int jmlkartu;
int f_akses[10];
//============
void Red_ON(){
  digitalWrite(LED_R,HIGH);
  digitalWrite(LED_G,LOW);
  digitalWrite(LED_B,LOW);
}
//============
void Green_ON(){
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,HIGH);
  digitalWrite(LED_B,LOW);
}
//============
void Blue_ON(){
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,LOW);
  digitalWrite(LED_B,HIGH);
}
//============
void readRFID(byte *buffer, byte bufferSize)
{
  ID_TAG="";
  for(byte i = 0;i<bufferSize; i++)
  {
    ID_TAG=ID_TAG+String(buffer[i], HEX);
  }
  digitalWrite(BUZZER,HIGH);
  delay(300);
  digitalWrite(BUZZER,LOW);
}
//=======================
void iot_rfid()
{
  Blue_ON(); 
  if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()){
    return;
    } 
  readRFID(mfrc522.uid.uidByte, mfrc522.uid.size);
  f_cek=0; 
  //---------------------------------------
  for(k=1;k<jmlkartu+1;k++){
    if(ID_TAG==ID[k]){  
      f_cek=1;
      if(f_akses[k]==1){    
        Serial.print("Akses diberikan untuk Kartu : ");
        Serial.println(k);    
        Blynk.virtualWrite(V0, "Akses diberikan untuk Kartu "+String(k));
        Green_ON();
        digitalWrite(relay,LOW);    
        delay(5000);
        digitalWrite(relay,HIGH);
        break;
      }
      else{
        Serial.print("Kartu : ");
        Serial.print(k); 
        Serial.println(" mencoba masuk, akses ditutup !");   
        Blynk.virtualWrite(V0, "Kartu "+String(k)+" GAGAL, AKSES KARTU TIDAK BISA, SILAHKAN COBA LAGI, TERIMA KASIH");
        Red_ON();
        digitalWrite(BUZZER, HIGH);
        delay(5000);
        digitalWrite(BUZZER, LOW);
        break;
      }
    }
  }
  //-------------------------------------------------------
  if(f_cek==0){    
    Serial.println("Kartu Tidak Terdaftar !"); 
    Blynk.virtualWrite(V0, "Kartu tidak terdaftar mencoba masuk !" );
    Serial.println("Akses ditolak !");
    Blynk.virtualWrite(V0, "Akses ditolak !");
    Blynk.logEvent("kartu_ilegal", "Kartu tidak terdaftar mencoba masuk !");
    Red_ON();
    digitalWrite(BUZZER, HIGH);
    delay(5000);
    digitalWrite(BUZZER, LOW);    
  }  
}
//==================================
BLYNK_WRITE(V1)
{
  f_akses[1] = param.asInt(); 
}
//==================================
BLYNK_WRITE(V2)
{
  f_akses[2] = param.asInt(); 
}
//==================================
BLYNK_WRITE(V3)
{
  f_akses[3]= param.asInt(); 
}
//============================
void setup()
{
  //sesuaikan jumlah kartu RFID-nya
  jmlkartu=3;  
  ID[1]="f3263510";
  ID[2]="d38590aa";
  ID[3]="a37075a9";  
  //tambahkan ID kartu disini ...  
  Serial.begin(115200);
  delay(200);
  pinMode(relay, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  digitalWrite(relay,HIGH); 
  digitalWrite(LED_R,HIGH);
  digitalWrite(LED_G,LOW);
  digitalWrite(LED_B,LOW);
  Blynk.begin(auth, ssid, pass);  
  delay(1000);   
  SPI.begin();                // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  timer.setInterval(1000L, iot_rfid);
  
  delay(500);  
}
//=======================
void loop() { 
  Blynk.run(); 
  timer.run(); 
}
