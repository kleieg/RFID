#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

int cmd;
int numbyte;
int menu = 1;
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522


}
void loop() 
{
  if (menu) {
    Serial.println("1 - Karte lesen ( Karte muss bereits aufliegen!!)");
    Serial.println("2 - Karte schreiben");
    Serial.print("Auswahl:");
    menu = 0;
  }

  if (Serial.available() > 0) { // if there is data 
    numbyte = Serial.available();

    if (numbyte > 1){
      Serial.println("Eingabe zu lang!");  // tritt nie auf!
    }
    cmd =  Serial.read() - 48;


    switch (cmd) {
      case 1:
        // Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) 
        {
          Serial.println();
          Serial.println();
          Serial.println("keine Karte oder Karte bereits gelesen.");
          Serial.println();
          menu = 1;
          return;
        }
        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial()) 
        {
          Serial.println();
          Serial.println();
          Serial.println("?? unklar??");
          Serial.println();
          menu = 1;
          return;
        }

        mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
        
        Serial.println();
        Serial.println();
        Serial.println("Karte wird nur einmal gelesen!");
        Serial.println("zum erneuten lesen: entfernen und erneut auflegen.");
        Serial.println();
        Serial.println();
        menu = 1;
      break;
      default:
        Serial.println();
        Serial.println();
        Serial.println("falsche Auswahl!");
        Serial.println();
        Serial.println();
        menu = 1;
    }
  }
} 

