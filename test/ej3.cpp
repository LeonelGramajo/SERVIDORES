/*3. Dando el diagrama de conexión para un LED RGB de cátodo común, codificar un programa que permita seleccionar colores fijos predefinidos. Para este control se 
posee una página web con las siguientes características:
* Título de primer nivel “Selector de Color RGB”, centrado.
* Color de fondo del cuerpo negro.
* Una sección (span) con la leyenda “Color Actual: XXX”, con texto de color blanco. XXX será el nombre del color seleccionado (“Rojo”, “Verde”, “Azul”, “Blanco” o “Apagado”).
* Un contenedor (div) con display:flex y justify-content:center para alinear cinco botones en una fila.
* Cinco botones circulares (usando border-radius: 50%) de clase “btn_color”, con ancho y alto de 80px y un borde blanco de 3px. Cada botón tendrá un color de 
fondo distintivo: rojo, verde, azul, blanco y gris (#34495e) para apagar. 
Al hacer clic, se enviará la petición correspondiente, el LED RGB cambiará al color seleccionado y la página se recargará mostrando el nombre del nuevo color.

*/
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "EL_NOMBRE_DE_TU_WIFI";
const char* password = "LA_CONTRASENA_DE_TU_WIFI";

int rgb_color=0;

int r = 4;
int g = 5;
int b = 18;
String color="XXX";
// Servidor Asíncrono 
AsyncWebServer server(80);



// HTML -> no cambia
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <meta http-equiv="refresh" content="1" />

  <title>Selector</title>
  <style>
    body { background-color:#000000; text-align: center; padding-top: 50px;
     }
     
    .titulo { font-family: 'Arial Black', monospace; font-weight: bold; font-style: Impact; font-size: 45px; color: #ffffff; text-align:center;
    text-decoration: underline;           /* Subrayado */
  text-decoration-color: #2c3e50;       /* Color de la línea */
  text-decoration-thickness: 3px;       /* Grosor de la línea */
  text-underline-offset: 5px;           /* Separación del texto */}
  .contenedor {display:flex; justify-content:center; flex-direction: column;gap: 15px;}
  .btn_color {border-radius:50%;width:80px;height:80px;border-color:white;
  border-radius: 3px;cursor: pointer;font-family: 'Arial', sans-serif;
  font-size: 20px;        /* Tamaño de letra más grande */
  font-weight: bold; color: rgb(187, 163, 24);}
  .rojo {background-color: rgb(255, 0, 0);}
  .azul {background-color: rgb(0, 0, 255);}
  .blanco {background-color: rgb(255, 255, 255);}
  .gris {background-color:#34495e;}

  #color {

    font-weight: bold;
    font-size: 40px;
    color:white;
    margin-bottom: 25px;
}
  </style>
</head>
<body>
  <h1 class='titulo'>Seletor de Color RGB</h1>
  <div class = "contenedor";>
  <span id ="color">Color Actual: __COLOR__</span>
  <a href="'/rojo"><button class = "btn_color rojo">rojo</button></a>
    <a href="'/azul"><button class ="btn_color azul">azul</button></a>
    <a href="'/blanco"><button class ="btn_color blanco">blanco</button></a>
    <a href="'/gris"><button class ="btn_color gris">gris</button></a>
  </div>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
      pinMode(r, OUTPUT);
      pinMode(g, OUTPUT);
      pinMode(b, OUTPUT);

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

    pagina.replace("__COLOR__", color);
    
    request->send(200, "text/html", pagina);
  });
  server.on("/rojo", HTTP_GET, [](AsyncWebServerRequest *request) {

    rgb_color=1;    
    request->redirect("/");
  });
  server.on("/verde", HTTP_GET, [](AsyncWebServerRequest *request) {
    String pagina = pagina_template; //hago una variable para no modificar a la original
    rgb_color=2;    
    
    request->redirect("/");
    
  });
  server.on("/azul", HTTP_GET, [](AsyncWebServerRequest *request) {
    String pagina = pagina_template; //hago una variable para no modificar a la original
    rgb_color=3;    
    
    request->redirect("/");
    
  });
  server.on("/blanco", HTTP_GET, [](AsyncWebServerRequest *request) {
    
    rgb_color=4;    
    request->redirect("/");
    
  });
  server.on("/gris", HTTP_GET, [](AsyncWebServerRequest *request) {
    
    rgb_color=5;    
    
    request->redirect("/");
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {       //apagado
    rgb_color = 0;
  
    request->redirect("/"); //lo mando a la ruta (/)
  });
  // Iniciar servidor
  server.begin();
}

void loop() {
  switch(rgb_color){
    case 0:
    analogWrite(r, 0);
    analogWrite(g, 0);
    analogWrite(b, 0);
    break;
    case 1:
    analogWrite(r, 255);
    analogWrite(g, 0);
    analogWrite(b, 0);
    break;
    case 2:
    analogWrite(r, 0);
    analogWrite(g, 255);
    analogWrite(b, 0);
    break;
    case 3:
    analogWrite(r, 0);
    analogWrite(g, 0);
    analogWrite(b, 255);
    break;
    case 4:
    analogWrite(r, 255);
    analogWrite(g, 255);
    analogWrite(b, 255);
    break;
    case 5:
    analogWrite(r, 80);
    analogWrite(g, 80);
    analogWrite(b, 80);
    default:
            break;
}
 }
