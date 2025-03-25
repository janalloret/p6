#include <SPI.h>
#include <SD.h>
/*SPI.h: Aquesta biblioteca gestiona la comunicació Serial Peripheral Interface (SPI), que és un protocol utilitzat per a la comunicació amb dispositius externs, com la targeta SD en aquest cas.

SD.h: Aquesta biblioteca proporciona funcions per treballar amb la targeta SD, com obrir fitxers, llegir-los, escriure-hi, etc.
*/
File myFile;
// myFile: És una variable de tipus File que utilitzaràs per emmagatzemar i gestionar el fitxer que obres des de la targeta SD.

void setup()
{
  Serial.begin(9600);
  Serial.print("Iniciando SD ...");
/*Serial.begin(9600): Aquesta línia inicialitza la comunicació sèrie amb el port sèrie a una velocitat de 9600 bauds. Això permet veure els missatges que s'imprimeixen per la consola sèrie.

Serial.print("IniciLa funció loop() està buida, perquè el codi només fa una lectura del fitxer a la funció setup(). El codi no fa cap altra acció contínua, ja que la funció loop() no té codi dins.ando SD ..."): Envia un missatge per la consola sèrie que diu que s'està iniciant la targeta SD.
*/
  if (!SD.begin(4)) {
    Serial.println("No se pudo inicializar");
    return;
  }
/*SD.begin(4): Inicialitza la targeta SD. El número 4 es refereix al pin de la placa ESP32 on es troba la línia CS (Chip Select) de 
la targeta SD. Si la inicialització falla, imprimeix el missatge "No se pudo inicializar" per la consola sèrie i atura l'execució del codi amb return.
*/
  Serial.println("inicializacion exitosa");
//Si la inicialització és correcta, es mostrarà aquest missatge per la consola sèrie.

  myFile = SD.open("archivo.txt");
  if (myFile) {
    Serial.println("archivo.txt:");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("Error al abrir el archivo");
  }
/*
    myFile = SD.open("archivo.txt"): Aquí s'intenta obrir el fitxer anomenat archivo.txt des de la targeta SD.

    if (myFile): Si el fitxer s'obre correctament, entra en aquest bloc de codi.

        Serial.println("archivo.txt:"): Imprimeix per la consola sèrie que el fitxer s'ha obert correctament.
La funció loop() està buida, perquè el codi només fa una lectura del fitxer a la funció setup(). El codi no fa cap altra acció contínua, ja que la funció loop() no té codi dins.
        while (myFile.available()): Aquest bucle llegeix el fitxer mentre encara queden dades per llegir.

void loop(){
}            myFile.read(): Llegeix una byte de dades del fitxer.

            Serial.write(): Envia aquest byte a la consola sèrie per mostrar-lo.

        myFile.close(): Un cop llegit tot el contingut, es tanca el fitxer.

    Si no s'ha pogut obrir el fitxer, imprimeix el missatge "Error al abrir el archivo".
*/
void loop(){
}
// La funció loop() està buida, perquè el codi només fa una lectura del fitxer a la funció setup(). El codi no fa cap altra acció contínua, ja que la funció loop() no té codi dins.
