#include <ESP8266WiFi.h>

// Este código tem como objetivo fazer uma simulação
// de um controle reidencial.
// Aqui será criada uma aplicação que controlará
// a ESP. Para acessar basta adicionar o IP 
// gerado e informado na inicialização da placa.

// Para adicionar ou remover "luzes" basta seguir os passos: 
// 1- Declarar o pino
// 2- Definir estado inicial
// 3- Inicializar pinos
// 4- Adicionar GET ou POST
// 5- Adicionar botões


// ************* DECLARAÇÃO ****************
// Pinos que irão representar os cômodos
#define cozinha 5 // roxo
#define sala 4    // amarelo
#define musica 14 // azul

// Definição dos estados iniciais
String estado_cozinha = "DESLIGADO";
String estado_sala = "DESLIGADO";
String estado_musica = "DESLIGADO";

// Variáveis de tempo
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 1000;

// Definição da rede e IPs a serem utilizados
const char *ssid = "Lijesp";
const char *password = "24Nov92#";
IPAddress local_IP(192,168,0,192);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
WiFiServer server(80);

// Variáveis gerais
String header;

void setup() {
// ********* INICIALIZÃO DOS PINOS *********
  pinMode(cozinha, OUTPUT);
  pinMode(sala, OUTPUT);
  pinMode(musica, OUTPUT);

  digitalWrite(cozinha, LOW);
  digitalWrite(sala, LOW);
  digitalWrite(musica, LOW);

// *************** CONEXÃO ******************
  // Definição do baud (velocidade)
  Serial.begin(115200);
 	Serial.println();

  // Conecta à Rede Wifi indicada anteriormente
  WiFi.begin(ssid, password);
  
  // Espera até que seja conectado
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  // Exibe as informações de conexão
  Serial.println("");
  Serial.println("WiFi conectado ..!");
  Serial.print("IP obtido: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  WiFiClient client = server.available();
  // se um cliente se conectar será
  // exibido na Serial as informações
  // do cliente e será renderizado no 
  // navegador que acessar o IP do ESP8266
  // uma página para controlar a automação
  if (client) {
    currentTime = millis();
    previousTime = currentTime;

    Serial.println("New Client.");
    String currentLine = "";
    
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();

      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        
        if (c == '\n') {
          if (currentLine.length() == 0) {

  // Adicionar GETs ou POSTs para que a ação seja realizada
            // GET COZINHA
            if (header.indexOf("GET /cozinha/ligado") >= 0) {
              Serial.println("ACENDER COZINHA");
              estado_cozinha = "LIGADO";
              digitalWrite(cozinha, HIGH);
            }
            if (header.indexOf("GET /cozinha/desligado") >= 0) {
              Serial.println("DESLIGAR COZINHA");
              estado_cozinha = "DESLIGADO";
              digitalWrite(cozinha, LOW);
            }

            // GET SALA
            if (header.indexOf("GET /sala/ligado") >= 0) {
              Serial.println("ACENDER SALA");
              estado_sala = "LIGADO";
              digitalWrite(sala, HIGH);
            }
            if (header.indexOf("GET /sala/desligado") >= 0) {
              Serial.println("DESLIGAR SALA");
              estado_sala = "DESLIGADO";
              digitalWrite(sala, LOW);
            }

            //GET MUSICA
            if (header.indexOf("GET /musica/ligado") >= 0) {
              Serial.println("LIGAR MUSICA");
              estado_musica = "LIGADO";
              tone(musica,261);
              //digitalWrite(musica, HIGH);
            }
            if (header.indexOf("GET /musica/desligado") >= 0) {
              Serial.println("PARAR MUSICA");
              estado_musica = "DESLIGADO";
              digitalWrite(musica, LOW);
            }
  
  // Renderização da página
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            
            client.println(".button { background-color: #B84F4F; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #4CAF50;}</style></head>");
            
            client.println("<body><h1>CONTROLE PARA CASA</h1>");
            client.println("<hr>");
            // Botões de cada luz
            //COZINHA
            client.println("<body><h2>COZINHA</h2>");
            if (estado_cozinha == "DESLIGADO") {
              client.println("<p><a href=\"/cozinha/ligado\"><button class=\"button\">LIGAR</button></a></p>");
            } else {
              client.println("<p><a href=\"/cozinha/desligado\"><button class=\"button button2\">DESLIGAR</button></a></p>");
            }

            // SALA
            client.println("<body><h2>SALA</h2>");
            if (estado_sala == "DESLIGADO") {
              client.println("<p><a href=\"/sala/ligado\"><button class=\"button\">LIGAR</button></a></p>");
            } else {
              client.println("<p><a href=\"/sala/desligado\"><button class=\"button button2\">DESLIGAR</button></a></p>");
            }

            // MUSICA
            client.println("<body><h2>MUSICA</h2>");
            if (estado_musica == "DESLIGADO") {
              client.println("<p><a href=\"/musica/ligado\"><button class=\"button\">TOCAR</button></a></p>");
            } else {
              client.println("<p><a href=\"/musica/desligado\"><button class=\"button button2\">PARAR</button></a></p>");
            }
          }else {
            currentLine = "";
          }
        }else if (c != '\r') {
            currentLine += c;
        }
      }
    }
    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}