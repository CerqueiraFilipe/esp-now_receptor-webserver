#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

typedef struct myData {
  float x; float y; float z; int c;
} myData;

myData placas[3]; 
AsyncWebServer server(80); 

const char* ssid = "ESP32_MPU_Hub"; 
const char* password = "12345678"; 

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
  <title>Telemetria MILEAGE</title>
  <style>
    body { font-family: sans-serif; text-align: center; background: #eceff1; margin: 0; padding: 20px; }
    .container { display: flex; flex-wrap: wrap; justify-content: center; gap: 20px; }
    .card { background: white; border-radius: 12px; width: 250px; padding: 15px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); border-top: 5px solid #007bff; }
    .val { font-size: 1.2em; font-weight: bold; color: #007bff; }
  </style>
</head>
<body>
  <h2>Telemetria S.E. MILEAGE</h2>
  <div class="container">
    <div class="card"><h3>Sensor 1</h3><p>X: <span id="p1x" class="val">0</span> | Y: <span id="p1y" class="val">0</span> | Z: <span id="p1z" class="val">0</span></p></div>
    <div class="card"><h3>Sensor 2</h3><p>X: <span id="p2x" class="val">0</span> | Y: <span id="p2y" class="val">0</span> | Z: <span id="p2z" class="val">0</span></p></div>
    <div class="card"><h3>Sensor 3</h3><p>X: <span id="p3x" class="val">0</span> | Y: <span id="p3y" class="val">0</span> | Z: <span id="p3z" class="val">0</span></p></div>
  </div>
  <script>
    setInterval(function(){
      fetch('/status').then(response => response.json()).then(data => {
        for(let i=1; i<=3; i++){
          document.getElementById('p'+i+'x').innerHTML = data[i-1].x.toFixed(2);
          document.getElementById('p'+i+'y').innerHTML = data[i-1].y.toFixed(2);
          document.getElementById('p'+i+'z').innerHTML = data[i-1].z.toFixed(2);
        }
      });
    }, 200); // Atualiza a cada 200ms sem recarregar a página
  </script>
</body></html>)rawliteral";

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  myData temp;
  memcpy(&temp, incomingData, sizeof(temp)); 

  Serial.print(">>> Dados Recebidos do Sensor ID: ");
  Serial.println(temp.c);

  if (temp.c >= 1 && temp.c <= 3) {
    placas[temp.c - 1] = temp;
    
    Serial.print("Valores -> X: "); 
    Serial.print(temp.x);
    Serial.print(" | Y: "); 
    Serial.print(temp.y);
    Serial.print(" | Z: "); 
    Serial.println(temp.z);
    Serial.println("-----------------------");
  } else {
    Serial.println("ERRO: ID de sensor inválido recebido!");
  }
}

void setup() {
  Serial.begin(115200);

  Serial.print("MAC do Receptor: ");
  Serial.println(WiFi.macAddress());
  
  WiFi.mode(WIFI_AP_STA);

  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE); // Força canal 1
  esp_wifi_set_promiscuous(false);
  WiFi.softAP(ssid, password);

  Serial.println("Iniciando Receptor S.E. MILEAGE...");
  Serial.print("IP do Access Point: ");
  Serial.println(WiFi.softAPIP());

  if (esp_now_init() != ESP_OK){
    Serial.println("Erro ao inicializar ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv); 
  Serial.println("ESP-NOW pronto para receber dados.");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Rota que envia os dados das 3 placas em formato JSON
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<400> doc;
    JsonArray root = doc.to<JsonArray>();
    for(int i=0; i<3; i++){
      JsonObject obj = root.createNestedObject();
      obj["x"] = placas[i].x;
      obj["y"] = placas[i].y;
      obj["z"] = placas[i].z;
    }
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server.begin();
  Serial.println("Servidor Web iniciado.");
}

void loop() {}