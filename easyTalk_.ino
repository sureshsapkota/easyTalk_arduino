/**
* Read a card using a mfrc522 reader on your SPI interface
* Pin layout should be as follows (on Arduino Uno):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SS: Pin 10
* RST: Pin 9
*
* Script is based on the script of Miguel Balboa. 
* New cardnumber is printed when card has changed. Only a dot is printed
* if card is the same.
*
* @version 0.1
* @author Henri de Jong
* @since 06-01-2013
*/

#include <SPI.h>
#include <RFID.h>

#include <Wire.h>

#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN, RST_PIN); 

int buzzPin = 3;

// Setup variables:
    int serNum0;
    int serNum1;
    int serNum2;
    int serNum3;
    int serNum4;
//Korter nummer som man registerte før
int brikke[] = {6,114,204,59,131};
int card[] = {211,148,122,37,24};

void setup()
{ 
  pinMode(5, OUTPUT);//sette opp pin til LED lyset 
  Serial.begin(9600);
  SPI.begin(); 
  rfid.init();
  Wire.begin(); // join i2c bus (address optional for master)
  digitalWrite(5,LOW);//LED lyset lyser ikke i begynnelsen
}


byte x = 0;


void loop()

{
    //Hvis RFID-leser leser kort(altså kube legger på boks), finner systemet ut hvilken nummer kortet er
    if (rfid.isCard()) {      
       digitalWrite(5,HIGH);//LED lyset lyser
        if (rfid.readCardSerial()) {
            if (rfid.serNum[0] != serNum0
                && rfid.serNum[1] != serNum1
                && rfid.serNum[2] != serNum2
                && rfid.serNum[3] != serNum3
                && rfid.serNum[4] != serNum4
            ) {
                /* With a new cardnumber, show it. */
                serNum0 = rfid.serNum[0];
                serNum1 = rfid.serNum[1];
                serNum2 = rfid.serNum[2];
                serNum3 = rfid.serNum[3];
                serNum4 = rfid.serNum[4];
    //sjekker kort nummeren er lik hvilken person som man registerte før
    //deretter printer person nummer på terminal            
    if(checkCard(brikke)){
      Serial.print(3);
      return;
    }
    else if(checkCard(card)){
      Serial.print(2);
      return;
    }
    else{
      Serial.print(0);
      return;
    }              
             } else {
               /* If we have the same ID, just write a dot. */
             }
          }
          delay(500);
    }
    else if(!rfid.isCard()){//Hvis kube ikke lenge er i boks(altså kube blir tatt bordt)
      digitalWrite(5,LOW);//LED lyset lyser ikke
      /*if no card, we send a dot*/
      if(digitalRead(7) == HIGH){//Avbryt knappen blir trykket
          Serial.print('b'); 
          delay(1000); 
      }
      if(digitalRead(6) == HIGH){//Svar knappen blir trykket
          Serial.print('c'); 
          delay(1000); 
      }
      
      
      
               Serial.print("x");//det betyr prosessingen skal ikke gjøre noen
//sletter nummer for å starte og lese en ny kode
                serNum0 = 0;
                serNum1 = 0;
                serNum2 = 0;
                serNum3 = 0;
                serNum4 = 0;
                
    }
    
    rfid.halt();

}

//Sjekk om det er  kort nummer
boolean checkCard(int a[]){
  
    if(rfid.serNum[0] == a[0]
                && rfid.serNum[1] == a[1]
                && rfid.serNum[2] == a[2]
                && rfid.serNum[3] == a[3]
                && rfid.serNum[4] == a[4]){
      return true;
    }
    else {
      return false;
    }
}
