//ejercicio 1
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "EL_NOMBRE_DE_TU_WIFI";
const char* password = "LA_CONTRASENA_DE_TU_WIFI";


const int ventilador = 25; 
String velocidad="XXXX";
// Servidor Asíncrono 
AsyncWebServer server(80);



// HTML -> no cambia
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>Control venti</title>
  <style>
    body { background-color: #ecf0f1; text-align: center; padding-top: 50px; }
    .titulo { font-family: 'Arial Black', monospace; font-weight: bold; font-style: italic; font-size: 40px; color: #2c3e50; }
    .boton { width: 180px; height: 60px; background-color: rgb(118, 215, 240); color: black; border: 1px solid black; border-radius: 15px; font-size: 18px; font-weight: bold; cursor: pointer; margin-top: 20px;}
    .boton A{ width: 180px; height: 60px; background-color: #c0392b;}
    .boton B{ width: 180px; height: 60px; background-color: #2980b9;}
    .boton C{ width: 180px; height: 60px; background-color: #27ae60;} 
    .boton D{ width: 180px; height: 60px; background-color: #f39c12;}

  </style>
</head>
<body>
  <h1 class='titulo'>CONTROL DE VENTILADOR</h1>
  <p><span>Velocidad actual:  <P>__MODO__</P></span></p>
  <a href="'/off"><button class ="boton A">Apagado</button></a>
  <a href="'/low"><button class ="boton B">Bajo</button></a>
  <a href="'/medium"><button class ="boton C">Medio</button></a>
  <a href="'/high"><button class ="boton D">Alto</button></a>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(ventilador, OUTPUT);
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // Conexion wifi como antes
 int timeout = 20; 
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFallo la conexion. Reiniciando...");
    delay(1000);
    ESP.restart();
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("Dirección IP: http://");
  Serial.println(WiFi.localIP());


  // Defino rutas de cada peticion para el sv asíncrono
  // La función ahora recibe un puntero a la petición (request)
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String pagina = pagina_template; //hago una variable para no modificar a la original

    pagina.replace("__MODO__", velocidad);
    
    request->send(200, "text/html", pagina);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    velocidad="OFF";
    analogWrite(ventilador,0);
    request->redirect("/"); //lo mando a la ruta (/)
  });
  server.on("/low", HTTP_GET, [](AsyncWebServerRequest *request) {
    velocidad="LOW";
    analogWrite(ventilador,50);
    
    
    request->redirect("/"); //lo mando a la ruta (/)
  });
  server.on("/medium", HTTP_GET, [](AsyncWebServerRequest *request) {
    velocidad="MEDIUM";
    analogWrite(ventilador,127);
    
    
    request->redirect("/"); //lo mando a la ruta (/)
  });
  server.on("/high", HTTP_GET, [](AsyncWebServerRequest *request) {
    velocidad="HIGH";
    analogWrite(ventilador,255);
    
    
    request->redirect("/"); //lo mando a la ruta (/)
  });

  // Iniciar servidor
  server.begin();
}

void loop() {

}