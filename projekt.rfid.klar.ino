#include <SPI.h>
#include <U8glib.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK);

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Skapa en instans av MFRC522.

Servo myservo; // Skapa ett servo-objekt för att styra en servo


int pos = 0;    // variabel för att lagra servo-position

bool locked = false; // En flagga för att hålla reda på om dörren är låst eller öppen

void setup()
{
  Serial.begin(9600);   // Initiera seriell kommunikation
  SPI.begin();      // Initiera SPI-bussen
  mfrc522.PCD_Init();   // Initiera MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  oled.setFont(u8g_font_helvB10);
  
  myservo.attach(7);  // Kopplat servon till pin 7
}

void loop(){
  updateOled("Place card ");
  // Letar efter nya kort
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Välj ett av korten
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
   // Visa UID på seriell monitor
  Serial.print("UID tag :");
  String content = "";
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

  // Jämför UID med det tillåtna kortet
  if (content.substring(1) == "4A BF FF BE") UID för de tillåtna kortet
  {
    Serial.println("Authorized access");
    Serial.println();
    
    // Berätta för servo att gå till positionen i variabeln 'pos'
    updateOled("Authorized access");
    locked = !locked;
    delay(1000);
  } else {
    Serial.println(" Access denied");
    updateOled(" Access denied");
    delay(2000);
  }

// Styr servo baserat på om dörren är låst eller öppen
  if (locked == true) {
    myservo.write(90);
    updateOled("Öppen");
    
    

  } else {
    myservo.write(0);
    updateOled("låst");
   
    
    }
// Uppdatera OLED-skärmen med given text
}
void updateOled(String text) {
  oled.firstPage();
  do {
    oled.drawStr(0, 16, text.c_str());
  } while (oled.nextPage());
}
