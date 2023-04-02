#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
/* Create an instance of MIFARE_Key */
MFRC522::MIFARE_Key key;   

MFRC522::StatusCode status;

/* Set the block to which we want to write data */
/* Be aware of Sector Trailer Blocks */
int blockNum = 4;  
/* Create an array of 16 Bytes and fill it with data */
/* This is the actual data which is going to be written into the card */
byte blockData [16] = {0x13,0x37,0xB3,0x47,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


int cmd;
int menu = 1;
int read;
int track;
int dir;
int modus;



int getCMD()
{
  while (true) {
    if (Serial.available() > 0) { // if there is data

    cmd =  Serial.read() - 48;
    return (cmd);
    }
  }
}

int getDirectory()
{
  Serial.print("\nOrdner eingeben ( 1-9):");
  while (true) {
    if (Serial.available() > 0) { // if there is data

    cmd =  Serial.read() - 48;
    if (cmd > 0  and cmd < 10 ) return (cmd);
    }
  }
}

int getTrack()
{
  
  Serial.print("\nTrack eingeben ( 1-9):");
  while (true) {
    if (Serial.available() > 0) { // if there is data

    cmd =  Serial.read() - 48;
    if (cmd > 0  and cmd < 10 ) return (cmd);
    }
  }
}
 
void WriteDataToBlock(int blockNum, byte blockData[]) 
{
/* Authenticating the desired data block for write access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed for Write: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
return;
  }
else
  {
    Serial.println("Authentication success");
  }
/* Write data to the block */
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Writing to Block failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
return;
  }
else
  {
    Serial.println("Data was written into Block successfully");
  }
}


void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
}


void loop() 
{
  if (menu) {
    Serial.println("Karte auflegen und auswählen!!)");
    Serial.println("1 - Karte lesen");
    Serial.println("2 - Karte schreiben");
    Serial.print("Auswahl:");
    menu = 0;
  }

  cmd = getCMD();
  Serial.println(cmd);


  switch (cmd) {
    case 1: {
      // Look for new cards
      if ( ! mfrc522.PICC_IsNewCardPresent()) 
      {
        Serial.println("\n\nkeine Karte.\n");
        menu = 1;
        return;
      }
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) 
      {
        Serial.println("\n\n?? unklar in 1 ??\n");
        menu = 1;
        return;
      }

      mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
      Serial.println("\n\n");
      menu = 1;
      mfrc522.PCD_Init();   // Initiate MFRC522
    }
    break;
    case 2: { 

      read = 1;

      while (read) {
        Serial.println("\n\nModus wählen");
        Serial.println("1 - Hörspiel-Modus: Eine zufällige Datei aus dem Ordner wiedergeben (Es wird nur ein Titel abgespielt)");
        Serial.println("2 - Album-Modus:    Den kompletten Ordner der Reihe nach wiedergeben");
        Serial.println("3 - Party-Modus:    Den Ordnerzufällig wiedergeben (jede Datei nur einmal)");
        Serial.println("4 - Einzel-Modus:   Eine bestimmte Datei im Ordner wiedergeben");
        Serial.println("5 - Hörbuch-Modus:  Den kompletten Ordner wiedergeben, der Fortschritt wird gespeichert");
        Serial.println("6 - Adminfunktionen:Die Karte wird als Admin-Karte konfiguriert, um damit in das Admin-Menü zu gelangen.");
        Serial.print("Auswahl:");

        modus = getCMD();
        Serial.println(modus);
        
        switch (cmd) {
          case 1:
          case 2:
          case 3:
          case 4:
          case 5: {
            Serial.print("\nOrdner eingeben ( 1-9):");
            dir = getCMD();
            Serial.println(dir);
            Serial.print("\nTrack eingeben ( 1-9):");
            track = getCMD();
            Serial.println(track);

            char* pDir=(char*)&dir;
            blockData [5] = pDir[0];
            char* pModus=(char*)&modus;
            blockData [6] = pModus[0];
            char* pTrack=(char*)&track;
            blockData [7] = pTrack[0];
            read = 0;
          }
          break;
          case 6: { 
            blockData [5] = 0x00;
            blockData [6] = 0xFF;
            blockData [7] = 0x00;
            read = 0;

          } 
          break;
          default:
            Serial.println("\n\nfalsche Auswahl!\n");
        }
      }

      //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      /* Prepare the ksy for authentication */
      /* All keys are set to FFFFFFFFFFFFh at chip delivery from the factory */
      for (byte i = 0; i < 6; i++)
        {
          key.keyByte[i] = 0xFF;
        }
      /* Look for new cards */
      /* Reset the loop if no new card is present on RC522 Reader */
      if ( ! mfrc522.PICC_IsNewCardPresent())
        {
        Serial.println("\n\nkeine Karte.\n");
        menu = 1;
        return;
        }
      /* Select one of the cards */
      if ( ! mfrc522.PICC_ReadCardSerial()) 
        {
        Serial.println("\n\n?? unklar in 2 ??\n");
        menu = 1;
        return;
        }
        
        Serial.println("\n**Card Detected**");
      /* Print UID of the Card */
        Serial.print(F("Card UID:"));
      for (byte i = 0; i < mfrc522.uid.size; i++)
        {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
        }
        ;
      /* Print type of card (for example, MIFARE 1K) */
        Serial.print(F("\nPICC type: "));
        MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
        Serial.println(mfrc522.PICC_GetTypeName(piccType));
      /* Call 'WriteDataToBlock' function, which will write data to the block */
        
        Serial.println("\nWriting to Data Block...");
      WriteDataToBlock(blockNum, blockData);
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      Serial.println("\n\n");
      menu = 1;
      mfrc522.PCD_Init();   // Initiate MFRC522
    }
    break;
    default:
      Serial.println("\n\nfalsche Auswahl!\n");
      menu = 1;
  }
} 

