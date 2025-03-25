#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Pines SPI (ESP32-S3)
#define RFID_CS_PIN   5   // GPIO5
#define RST_PIN       4   // GPIO4

// Configuración de red WiFi

const char* ssid = "Nautilus"; // Enter your SSID here
 

const char* password = "20000Leguas"; // Enter your Password here

// Inicialización del lector RFID
MFRC522 rfid(RFID_CS_PIN, RST_PIN); // Constructor

// Servidor web en el puerto 80
AsyncWebServer server(80);

// Página web HTML que se mostrará en el navegador
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Lectura RFID</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
        h1 { color: #4CAF50; }
        .uid-display { font-size: 1.5em; font-weight: bold; color: #333; }
        .button { margin-top: 20px; padding: 10px 20px; background-color: #4CAF50; color: white; border: none; cursor: pointer; }
        .button:hover { background-color: #45a049; }
    </style>
</head>
<body>
    <h1>Lectura RFID</h1>
    <p>Acerca una tarjeta RFID al lector para ver el UID.</p>
    <div class="uid-display" id="uid">Esperando tarjeta...</div>
    <button class="button" onclick="refreshUID()">Refrescar UID</button>

    <script>
        function refreshUID() {
            fetch("/readUID")
                .then(response => response.text())
                .then(data => {
                    document.getElementById("uid").textContent = "UID: " + data;
                });
        }
        setInterval(refreshUID, 2000); // Actualiza el UID cada 2 segundos
    </script>
</body>
</html>
)rawliteral";

// Función para leer el UID de la tarjeta RFID utilizando PICC_ReadCardSerial()
String readLink() {
  String uid = "";
  // Comprobamos si hay una tarjeta nueva
  if (rfid.PICC_IsNewCardPresent()) {
    // Leemos el UID de la tarjeta
    if (rfid.PICC_ReadCardSerial()) {
      // Construimos el UID como una cadena hexadecimal
      for (byte i = 0; i < rfid.uid.size; i++) {
        uid += String(rfid.uid.uidByte[i], HEX);
        if (i < rfid.uid.size - 1) {
          uid += " ";
        }
      }
      return uid;
    }
  }
  return "No hay tarjeta presente";
}

void setup() {
  Serial.begin(115200);
  delay(2000);  // Retardo para la inicialización

  // Conectar al Wi-Fi
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);

  int attempts = 0;
  // Espera hasta que el ESP32 se conecte a Wi-Fi o intente por un tiempo máximo
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);  // Esperar 1 segundo
    attempts++;
    Serial.print(".");
    
    if (attempts >= 30) {  // 30 intentos = 30 segundos
      Serial.println(" No se pudo conectar a Wi-Fi.");
      return;
    }
  }

  // Si la conexión es exitosa
  Serial.println("Conectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Inicializa SPI para el lector RFID
  SPI.begin(12, 13, 11, RFID_CS_PIN); // SCK=12, MISO=13, MOSI=11, SS=5
  rfid.PCD_Init();
  Serial.println("Acerca una tarjeta RFID...");

  // Configura el servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlPage);
  });

  // Ruta para leer el UID
  server.on("/readUID", HTTP_GET, [](AsyncWebServerRequest *request){
    String uid = readLink();  // Llamada a la función alternativa para leer el UID
    request->send(200, "text/plain", uid);
  });

  // Inicia el servidor
  server.begin();
}

void loop() {
  // Detección mejorada
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Detenemos la tarjeta RFID después de leer
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}


