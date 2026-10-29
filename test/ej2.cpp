/*2. Codificar un programa que simule un monitor de luz ambiental mediante un LDR. Para este control se posee una página web con las siguientes características:
* Título de primer nivel con leyenda “Monitor de Luz Ambiental”, con fuente ‘Segoe UI’ y centrado.
* El color de fondo del cuerpo (body) deberá cambiar dinámicamente según la luz medida: #2c3e50 (azul oscuro) para noche, #f1c40f (amarillo) para día,
 y #e67e22 (naranja) para atardecer, con texto de color blanco para todos los casos.
* Una división de clase “lectura” con un borde blanco de 3px punteado (dotted), radio de borde de 20px y un padding de 25px.
* Dentro de esta división, una sección (span) mostrará la leyenda “Nivel de Luz: XXX%”, donde XXX es el valor del LDR mapeado a un porcentaje. 
El valor porcentual deberá estar en negrita y tener un tamaño de fuente de 40px.
* La página se actualizará con los nuevos valores de luminosidad y color de fondo cada vez que el usuario la recargue.
*/
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "EL_NOMBRE_DE_TU_WIFI";
const char* password = "LA_CONTRASENA_DE_TU_WIFI";


const int LDR = 25; 
String Luz="XXXX", color="white";
int NivelLuz = 0;
// Servidor Asíncrono 
AsyncWebServer server(80);



// HTML -> no cambia
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <meta http-equiv="refresh" content="1" />

  <title>Monitor Luz Ambiental</title>
  <style>
    body { background-color:__COLOR__; text-align: center; padding-top: 50px;
      transition: background-color 0.5s ease;
     }
     
  .lectura{border:  dotted 3px solid rgb(255, 255, 255); border-radius: 20px; padding: 25px;}
    .titulo { font-family: 'Arial Black', monospace; font-weight: bold; font-style: Segoe UI; font-size: 40px; color: #ffffff; }
    #nivelLuz {
    font-weight: bold;
    font-size: 40px;
}
  </style>
</head>
<body>
  <h1 class='titulo'>Monitor de Luz Ambiental</h1>
  <span id ="nivelLuz">Nivel de Luz: __LDR__</span>
  <a href="'/azul"><body class = "noche"></a>
    <a href="'/amarillo"><body class = "dia"></a>
      <a href="'/naranja"><body class = "atardecer"></a>  
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(LDR, OUTPUT);
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

    pagina.replace("__LDR__", Luz);
    pagina.replace("__COLOR__", color);
    
    request->send(200, "text/html", pagina);
  });

  // Iniciar servidor
  server.begin();
}

void loop() {
  NivelLuz = analogRead(LDR);
  Luz=NivelLuz;
  int porcentaje = ((NivelLuz / 1023) * 100);
if(porcentaje >30){
  color="#2c3e50";
}
if(porcentaje>70){
  color="#e67e22";
}
else{
  color="#f1c40f";
}
 
 
}
