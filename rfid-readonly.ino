/*
 * 
 * All the resources for this project: https://www.hackster.io/Aritro
 * Modified Theodore Meimarakis
 * 
 * 
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <Wire.h>

 
#define SS_PIN 10
#define RST_PIN 9

#define Relay_PIN 8
#define Motion_PIN 2

// Greek characters for the I2C display

byte Gama[8] = {
                  B11111,
                  B10000,
                  B10000,
                  B10000,
                  B10000,
                  B10000,
                  B10000,
                  B00000,
};

byte Delta[8] = {
                 B00100,
                 B01010,
                 B01010,
                 B10001,
                 B10001,
                 B10001,
                 B11111,
};

byte Thita[8] = {
                  B01110,
                  B10001,
                  B10001,
                  B11111,
                  B10001,
                  B10001,
                  B01110,
                  B00000,
};

byte Lamda[8] = {
                 B00100,
                 B01010,
                 B01010,
                 B10001,
                 B10001,
                 B10001,
                 B10001,
};

byte Ksi[8] = {
                 B11111,
                 B00000,
                 B00000,
                 B01110,
                 B00000,
                 B00000,
                 B11111,
};

byte Pi[8] = {
                 B11111,
                 B10001,
                 B10001,
                 B10001,
                 B10001,
                 B10001,
                 B10001,
};

byte Sigma[8] = {
                 B11111,
                 B01000,
                 B00100,
                 B00010,
                 B00100,
                 B01000,
                 B11111,
};

byte Fi[8] = {
                 B00100,
                 B01110,
                 B10101,
                 B10101,
                 B10101,
                 B01110,
                 B00100,
};

byte Psi[8] = {
                 B10001,
                 B10101,
                 B10101,
                 B10101,
                 B01110,
                 B00100,
                 B00100,
};

byte Omega[8] = {
                 B01110,
                 B10001,
                 B10001,
                 B10001,
                 B01010,
                 B01010,
                 B11011,
};

char *acceptedCards[] = {"04 3F 46 1A 1F 5C 80",
"05 86 FF 17 AA 00 00",
"04 D1 49 82 75 63 80",
"04 D1 54 82 75 63 80",
"04 4C 46 1A 1F 5C 80",
"04 3F 46 1A 1F 5C 80",
"05 8E EB C8 D3 40 00",
"05 8A B4 54 6B 01 00",
"05 8C AA 97 32 21 00",
"A7 FC 8C F4",
"07 F7 1A F4",
"D4 BA 55 2E",
"77 3E 60 F4",
"84 0A 59 2E",
"91 C2 E5 2B"
};


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

LiquidCrystal_I2C lcd (0x27, 16, 2);

bool grandAccess = false; 


int i = 0;
int k = 0;
int rip_Val = 0;
int rip_State = LOW;
long previousMillis = 0; 
unsigned long currentMillis = 0;

void convert2Greek(char *phrase, int x, int y) {

lcd.setCursor(x,y);
for (int i=0;phrase[i]!= '\0';i++) {

 // Serial.println(phrase[i]);
          switch (phrase[i])
            {
              case 'G':

                   lcd.write(byte(0));
                    break;
              case 'D':

                   lcd.write(byte(1));
                    break;
              case 'U':

                   lcd.write(byte(2));
                    break;
              case 'L':

                   lcd.write(byte(3));
                    break;
              case 'J':

                   lcd.write(byte(4));
                    break;
              case 'P':

                   lcd.write(byte(5));
                    break;
              case 'S':

                   lcd.write(byte(6));
                    break;
              case 'V':

                   lcd.write(byte(7));
                    break;
              case 'R':

                   lcd.print("P");
                    break;

               default:
                lcd.print(phrase[i]);
                break;

           }
           //  delay(150);
             if (x++ == 15) { lcd.setCursor(0,1); } // lcd.scrollDisplayLeft(); }
        }

  }
 
void setup() 
{
   Serial.begin(9600);   // Initiate a serial communication
   SPI.begin();      // Initiate  SPI bus
   mfrc522.PCD_Init();   // Initiate MFRC522

   pinMode(Motion_PIN, INPUT);
   
   lcd.begin();
   lcd.backlight();
   convert2Greek("EKKINHSH        SYSTHMATOS...!",0,0);
   lcd.createChar(0, Gama); // Create 8 Greek characters EXCEPT Φ,Ψ
   lcd.createChar(1, Delta);
   lcd.createChar(2, Thita);
   lcd.createChar(3, Lamda);
   lcd.createChar(4, Ksi);
   lcd.createChar(5, Pi);
   lcd.createChar(6, Sigma);
   lcd.createChar(7, Omega);
   delay(1500);
   lcd.clear();
   convert2Greek("PARAKALV PLHSIASTE THN KARTA!",0,0);
   delay(1000);
  // lcd.noBacklight();
}
  
void loop() 
{
  //Light up screen in case of motion
  rip_Val = digitalRead(Motion_PIN);
 currentMillis = millis();
   
   if (rip_Val == HIGH) {
       previousMillis= millis();
       lcd.backlight(); 
       Serial.println(currentMillis);
    }
    else { // no motion detected
  
      if(currentMillis - previousMillis > 10000) {
        lcd.noBacklight();
        previousMillis = currentMillis; 
        } 
    }
   
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  //Show UID on serial monitor
  bool grandAccess = false; 
  lcd.clear();
  lcd.backlight();
  convert2Greek("ANAGNVRISH      KARTAS...",0,0);
  delay(500);
  lcd.clear();
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  for (int i = 0; i < sizeof(acceptedCards)/sizeof(acceptedCards[0]); i++) {
   
       Serial.println(acceptedCards[i]);
       if (content.substring(1) == acceptedCards[i]) //change here the UID of the card/cards that you want to give access
            {
              grandAccess = true;
              break;
            }
  }
       if (grandAccess) {
              
              lcd.clear();
              convert2Greek("KALVS HLUATE!!!",0,0);
              convert2Greek("H PORTA ANOIJE!",0,1);
              Serial.println("Authorized access");
              Serial.println();
              delay(4000);
              lcd.clear();
            
              
            }
           else   {
              lcd.clear();
              convert2Greek("H KARTA DEN     ANAGNVRISTHKE!",0,0);
              Serial.println(" Access denied");
              delay(3000);
              lcd.clear();
               }
  
      convert2Greek("PARAKALV PLHSIASTE THN KARTA!",0,0);
      
} 
