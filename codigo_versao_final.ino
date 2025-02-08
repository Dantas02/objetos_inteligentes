#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define sensor A0

const char* ssid = "<colocar_ssid>";
const char* password = "<colocar_senha>";
const char* mqtt_server = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* mqttTopic = "topico/umidade_solo";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

int Valvula;

unsigned long tempoSensor = millis();

const int leituraMinima = 500;   // Leitura correspondente a 100% de umidade
const int leituraMaxima = 1023;  // Leitura correspondente a 0% de umidade

float Umidade; 
float Porcentagem_Umidade; 

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a rede");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message recebida [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Aguardando conexao MQTT...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("topico/umidade_solo");
    } else {
      Serial.print("falhou. rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Valvula = D0;

  pinMode(Valvula, OUTPUT);
  pinMode(sensor, INPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if ((millis() - tempoSensor) >= 3000) {
    Send_Sensor();
    tempoSensor = millis();
  }

  // Declare a Umidade aqui para ser usada no loop
  if (Umidade > 800) {
    digitalWrite(Valvula, HIGH);
    delay(4000);
    digitalWrite(Valvula, LOW);
  }

  delay(2000);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void Send_Sensor() {
  // Leitura do sensor
  Umidade = analogRead(sensor);

  // Converte a leitura do sensor em porcentagem
  Porcentagem_Umidade = map(Umidade, leituraMinima, leituraMaxima, 100, 0);

  Serial.print("Porcentagem da Umidade: ");
  Serial.print(Porcentagem_Umidade);
  Serial.println(F("%"));

  // Formata a porcentagem com zero casas decimais
  sprintf(msg, "%.0f", Porcentagem_Umidade);
  client.publish("topico/umidade_solo", msg);
}
