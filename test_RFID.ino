
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, layout tipico arriba
#define SS_1_PIN        10         // Configurable, solo HIGH/LOW, diferente a SS 2
#define SS_2_PIN        8          // Configurable, solo HIGH/LOW, diferente a SS 1

#define NR_OF_READERS   2

byte ssPins[] = {SS_1_PIN, SS_2_PIN};

MFRC522 mfrc522[NR_OF_READERS];   // Crear una instancia MFRC522

/**
 * Inicializa
 */
void setup() {

  Serial.begin(9600); // Inicializar comunicación serial
  while (!Serial);    // No hacer nada si no hya puerto serial

  SPI.begin();        // Inicializar bus SPI

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Inicializa las tarejtas MFRC522
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}

/**
 * Loop princiapl.
 */
void loop() {

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Busca nuevas tarjetas

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      Serial.print(F("Reader "));
      Serial.print(reader);
      // Muestra detalles del PICC (esto es: el tag/tarjeta)
      Serial.print(F(": Card UID:"));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
      Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));

      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Detener encriptación en PCD
      mfrc522[reader].PCD_StopCrypto1();
    } //if (mfrc522[reader].PICC_IsNewC
  } //for(uint8_t reader
}

/**
 * Rutina para cambiar un valor en bytes a hexadecimal
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
