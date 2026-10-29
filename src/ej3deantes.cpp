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


const char* ssid = "CESJT";
const char* password = "itisjtsmg";

int sem_modo=0;

unsigned long t_delay_lento_rojo = 250;
unsigned long t_delay_lento_amarillo = 500;         //milis de lento
unsigned long t_delay_lento_verde = 750;

unsigned long t_delay_normal_rojo = 45000;
unsigned long t_delay_normal_amarillo = 2000;        //milis de normal
unsigned long t_delay_normal_verde = 65000;

unsigned long t_delay_rapido_rojo = 20000;
unsigned long t_delay_rapido_amarillo = 1000;        //milis de rapido
unsigned long t_delay_rapido_verde = 40000;

unsigned long t_delay_emergencia = 1500;            //milis de emergencia

unsigned long t_previo = 0;
unsigned long t_actual = 0;

int semaforo[3] = { 4, 5, 14}; //4 rojo, 5 amarillo, 14 ver

void setup() {
 
    for(int i=0;i<3;i++){
      pinMode(semaforo[i], OUTPUT);
    }
 
}

void loop() {
    t_actual = millis();

    if(t_actual - t_previo <= t_delay_lento_rojo){
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(14, LOW);
      t_previo = t_actual;
    }
    
    else if((t_actual - t_previo) <= (t_delay_lento_amarillo+t_delay_lento_rojo)){
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(14, LOW);
        t_previo = t_actual;
    }
    
    else  if(t_actual - t_previo <= (t_delay_lento_verde+t_delay_lento_amarillo+t_delay_lento_rojo)){
        t_previo = t_actual;
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(14, HIGH);  
        t_previo = t_actual;
      }
      else 
        t_previo = t_actual;
      
   
 }
