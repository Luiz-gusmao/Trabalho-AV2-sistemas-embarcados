#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <WebSocketsServer.h>
#define D1 5
const char *ssid = "VIRUS";
const char *password = "14151610";
String btn;
WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);
//local onde é gerado o arquivo html para ser enviado ao client
const char PaginaHTML[] PROGMEM = R"=====(




<!DOCTYPE html>
<html>
<script>
var connection = new WebSocket('ws://'+location.hostname+':81/');
function button_1_on()
{
  console.log("ON");
  connection.send("ON");
}
function button_1_off()
{
console.log("OFF");
connection.send("OFF");
}
</script>
<body>
<center>
<h1>Interruptor</h1>
<button onclick= "button_1_on()" >On</button><button onclick="button_1_off()" >Off</button>
</center>
</body>
</html>




)=====";


void paginaInicial()
{
  server.send(200, "text/html", PaginaHTML); //Envia a pagina
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      {

        Serial.printf("[%u] get Text: %s\n", num, payload);
      String state = String((char*)( payload));
      Serial.println(state);

      if(state == "OFF"){
        digitalWrite(D1,LOW);
      }

      if(state == "ON"){
        digitalWrite(D1,HIGH);
      }
        
        break;
      }
  }
}

void setup() {
  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);
  Serial.begin (115200);
  WiFi.begin (ssid, password);//conecta ao wifi

    //return;

  while (WiFi.status() != WL_CONNECTED) { //espera a conexão
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());

  server.on("/",paginaInicial); 
  server.begin(); 
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  server.handleClient();
  webSocket.loop();

  
 
  
}
