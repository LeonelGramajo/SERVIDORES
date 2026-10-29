/*3. Codificar un servidor web síncrono que simule un controlador de un semáforo con 4 modos: lento, normal, rápido y emergencia. El modo lento consiste de un rojo de 
70 segundos, amarillo de 3 segundos y un verde de 50 segundos. El modo normal consiste de un rojo de 45 segundos, amarillo de 2 segundos y verde de 65 segundos. 
El modo rápido consiste de un rojo de 20 segundos, amarillo de 1 segundo y verde de 40 segundos. El modo emergencia consiste de rojo y amarillo parpadeante cada 1,5 segundos.
Para este control se posee una página web con las siguientes características:
◦ Título de primer nivel con leyenda “Semáforo”, todo con mayúsculas, con fuente ‘Impact’, negrita y subrayada de 45px. Utilizar la clase “titulo”.
◦ Color del fondo del cuerpo blanco.
◦ Todo centrado.
◦ 4 botones, uno debajo del otro, de ancho 200px y alto 200px, de color de fondo rojo (lento), amarillo (normal), verde (rápido) y naranja (emergencia); 
color de texto negro con tamaño de fuente de 23px, borde negro de 3px sólido de radio 100px. Utilizar clase genérica “boton” y clases individuales “lento”, “normal”, “rápido” y “emergencia”.
◦ Abajo de todo agregar una sección (span) con la leyenda “Modo: XXX” y reemplazar el XXX por el modo seleccionado en cuestión, el modo solo en negrita.
*/
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "EL_NOMBRE_DE_TU_WIFI";
const char* password = "LA_CONTRASENA_DE_TU_WIFI";

int sem_modo=0;

unsigned long t_delay_lento_rojo = 70000;
unsigned long t_delay_lento_amarillo = 3000;         //milis de lento
unsigned long t_delay_lento_verde = 50000;

unsigned long t_delay_normal_rojo = 45000;
unsigned long t_delay_normal_amarillo = 2000;        //milis de normal
unsigned long t_delay_normal_verde = 65000;

unsigned long t_delay_rapido_rojo = 20000;
unsigned long t_delay_rapido_amarillo = 1000;        //milis de rapido
unsigned long t_delay_rapido_verde = 40000;

unsigned long t_delay_emergencia = 1500;            //milis de emergencia

unsigned long t_previo = 0;
unsigned long t_actual = 0;

int semaforo[3] = { 4, 5, 14}; //4 rojo, 5 amarillo, 14 verde
bool parpadeo;
String modo="XXX";
// Servidor Asíncrono 
AsyncWebServer server(80);



// HTML -> no cambia
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <meta http-equiv="refresh" content="1" />

  <title>Semaforo</title>
  <style>
    body { background-color:#ffffff; text-align: center; padding-top: 50px;
     }
     
    .titulo { font-family: 'Arial Black', monospace; font-weight: bold; font-style: Impact; font-size: 45px; color:rgb(90, 9, 9); 
    text-decoration: underline;           /* Subrayado */
  text-decoration-color: #2c3e50;       /* Color de la línea */
  text-decoration-thickness: 3px;       /* Grosor de la línea */
  text-underline-offset: 5px;           /* Separación del texto */}
  .boton { width: 200px; height: 200px; background-color: __COLOR__; color: black; border: 3px solid black; border-radius: 100px; font-size: 23px; font-weight: bold; cursor: pointer; margin-top: 20px;}
  .lento {background-color: red;}
  .normal {background-color: yellow;}
  .rapido {background-color: green;}
  .emergencia {background-color: orange;}

  #semarforo {
    font-weight: bold;
    font-size: 40px;
}
  </style>
</head>
<body>
  <h1 class='titulo'>SEMÁFORO</h1>
  <span id ="semarforo">Modo: __MODO__</span>
  <a href="'/slow"><button class = "lento">Lento</a>
    <a href="'/normal"><button class ="normal">Normal</button></a>
    <a href="'/fast"><button class ="rapido">Rapido</button></a>
    <a href="'/Emergency"><button class ="emergencia">Emergencia</button></a>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
    for(int i=0;i<3;i++){
      pinMode(semaforo[i], OUTPUT);
    }
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

    pagina.replace("__MODO__", modo);
    
    request->send(200, "text/html", pagina);
  });
  server.on("/slow", HTTP_GET, [](AsyncWebServerRequest *request) {

    sem_modo=0;    
    request->redirect("/");
  });
  server.on("/normal", HTTP_GET, [](AsyncWebServerRequest *request) {
  
    sem_modo=1;    
    
    request->redirect("/");
    
  });
  server.on("/fast", HTTP_GET, [](AsyncWebServerRequest *request) {
    
    sem_modo=2;    
    request->redirect("/");
    
  });
  server.on("/emergency", HTTP_GET, [](AsyncWebServerRequest *request) {
    
    sem_modo=3;    
    
    
  });

  // Iniciar servidor
  server.begin();
}

void loop() {
  switch(sem_modo){
    case 0:
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(18, LOW);
    if(t_actual - t_previo >= t_delay_lento_rojo){
      t_previo = t_actual;
      }
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(18, LOW);
    
      if(t_actual - t_previo >= t_delay_lento_amarillo){
        t_previo = t_actual;
        }
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(18, HIGH);  
      
      if(t_actual - t_previo >= t_delay_lento_verde){
        t_previo = t_actual;
      }
      break;
    case 1:
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(18, LOW);
    if(t_actual - t_previo >= t_delay_normal_rojo){
      t_previo = t_actual;
      }
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(18, LOW);
      if(t_actual - t_previo >= t_delay_normal_amarillo){
        t_previo = t_actual;
        }
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(18, HIGH);  
        if(t_actual - t_previo >= t_delay_normal_verde){
          t_previo = t_actual;
        }
            break;
    case 2:
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(18, LOW);
    if(t_actual - t_previo >= t_delay_rapido_rojo){
      t_previo = t_actual;
      }
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(18, LOW);
      if(t_actual - t_previo >= t_delay_rapido_amarillo){
        t_previo = t_actual;
        }
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(18, HIGH);  
        if(t_actual - t_previo >= t_delay_rapido_verde){
          t_previo = t_actual;
        }
            break;
    case 3:
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    if(t_actual - t_previo >= t_delay_emergencia){
      t_previo = t_actual;
    }  
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
            break;
    default:
            break;
}
 }
