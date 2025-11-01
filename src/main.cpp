/*/*

 * ESP32 Temperature & Humidity Monitor v2.0 PRO * ESP32 Temperature & Humidity Monitor

 *  * 

 * Sistema IoT profesional de monitoreo con OTA, WiFiManager y observabilidad completa. * Sistema IoT para monitoreo de temperatura y humedad en tiempo real

 *  * con servidor web integrado y API REST.

 * Features v2.0: * 

 * - OTA (Over-The-Air updates) via ArduinoOTA * Características:

 * - WiFiManager para configuración sin hardcodear credenciales * - Lectura de sensor DHT22 (temperatura y humedad)

 * - SSE (Server-Sent Events) para tiempo real * - Servidor web con dashboard responsive

 * - Historial de lecturas (últimas 50) con gráfico Chart.js * - API REST para obtener datos en formato JSON

 * - Export CSV de histórico * - Actualización automática cada 2 segundos

 * - mDNS (esp32-temp.local) * - Indicador LED para estado de conexión WiFi

 * - NTP timestamps UTC * 

 * - Endpoints: /metrics (Prometheus), /healthz, /api/sensors, /api/history, /api/export * Hardware:

 *  * - ESP32 DevKit

 * Hardware: * - Sensor DHT22

 * - ESP32 DevKit + DHT22 (GPIO 4) * - LED (opcional)

 * - LED integrado (GPIO 2) * 

 *  * Conexiones:

 * Autor: Sebastian Lara | github.com/SEBASTIAN3451 * - DHT22 Data Pin -> GPIO 4

 * Fecha: Octubre 2025 * - LED -> GPIO 2 (LED integrado)

 */ * 

 * Autor: Sebastian Lara

#include <Arduino.h> * Fecha: Octubre 2025

#include <WiFi.h> */

#include <WiFiManager.h>

#include <ESPAsyncWebServer.h>#include <Arduino.h>

#include <ESPmDNS.h>#include <WiFi.h>

#include <ArduinoOTA.h>#include <ESPAsyncWebServer.h>

#include <time.h>#include <ESPmDNS.h>

#include <DHT.h>#include <time.h>

#include <ArduinoJson.h>#include <DHT.h>

#include <ArduinoJson.h>

// ==================== CONFIG ====================

#define DHT_PIN 4// ==================== CONFIGURACIÓN ====================

#define LED_PIN 2

#define DHT_TYPE DHT22// Configuración WiFi - CAMBIAR ESTOS VALORES

#define SENSOR_READ_INTERVAL 2000const char* ssid = "TU_WIFI_SSID";

#define HISTORY_SIZE 50const char* password = "TU_WIFI_PASSWORD";



// ==================== GLOBALS ====================// Pines

DHT dht(DHT_PIN, DHT_TYPE);#define DHT_PIN 4          // Pin del sensor DHT22

AsyncWebServer server(80);#define LED_PIN 2          // LED de estado (LED integrado del ESP32)

AsyncEventSource events("/events");#define DHT_TYPE DHT22     // Tipo de sensor DHT



struct Reading { float temp; float hum; time_t timestamp; };// Intervalo de lectura del sensor (milisegundos)

Reading history[HISTORY_SIZE];const unsigned long SENSOR_READ_INTERVAL = 2000;

int historyIndex = 0;

int historyCount = 0;// ==================== OBJETOS GLOBALES ====================



float temperature = 0.0, humidity = 0.0;DHT dht(DHT_PIN, DHT_TYPE);

unsigned long lastReadTime = 0;AsyncWebServer server(80);

AsyncEventSource events("/events");

// ==================== HTML with Chart.js ====================

const char index_html[] PROGMEM = R"rawliteral(// Variables para almacenar lecturas

<!DOCTYPE html>float temperature = 0.0;

<html lang="es">float humidity = 0.0;

<head>unsigned long lastReadTime = 0;

<meta charset="utf-8"/><meta name="viewport" content="width=device-width, initial-scale=1"/>time_t lastNtpSync = 0;

<title>🌡️ ESP32 Temp Monitor PRO</title>

<script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.0"></script>// ==================== PÁGINA WEB ====================

<style>

*{margin:0;padding:0;box-sizing:border-box}const char index_html[] PROGMEM = R"rawliteral(

body{font-family:'Segoe UI',sans-serif;background:linear-gradient(135deg,#667eea,#764ba2);min-height:100vh;padding:20px}<!DOCTYPE html>

.container{max-width:1000px;margin:0 auto;background:#fff;border-radius:20px;padding:30px;box-shadow:0 20px 60px rgba(0,0,0,0.3)}<html lang="es">

h1{text-align:center;color:#333;margin-bottom:30px}<head>

.cards{display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:20px;margin-bottom:30px}    <meta charset="UTF-8">

.card{background:linear-gradient(135deg,#f093fb,#f5576c);border-radius:15px;padding:20px;color:#fff;text-align:center}    <meta name="viewport" content="width=device-width, initial-scale=1.0">

.card h3{font-size:14px;opacity:0.9;margin-bottom:8px}    <title>Monitor de Temperatura ESP32</title>

.card .value{font-size:36px;font-weight:bold}    <style>

#chartContainer{margin:20px 0}        * { margin: 0; padding: 0; box-sizing: border-box; }

.btn{background:#667eea;color:#fff;border:none;padding:10px 20px;border-radius:8px;cursor:pointer;margin:5px}        

.btn:hover{background:#764ba2}        body {

.status{text-align:center;padding:10px;background:#f0f0f0;border-radius:8px;margin-top:15px}            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;

.dot{display:inline-block;width:10px;height:10px;border-radius:50%;background:#4caf50;animation:pulse 2s infinite;margin-right:6px}            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);

@keyframes pulse{0%,100%{opacity:1}50%{opacity:0.5}}            min-height: 100vh;

</style>            display: flex;

</head>            justify-content: center;

<body>            align-items: center;

<div class="container">            padding: 20px;

<h1>🌡️ Monitor de Clima PRO</h1>        }

<div class="cards">        

<div class="card" style="background:linear-gradient(135deg,#f093fb,#f5576c)">        .container {

<h3>Temperatura</h3><div class="value"><span id="temp">--</span>°C</div></div>            background: white;

<div class="card" style="background:linear-gradient(135deg,#4facfe,#00f2fe)">            border-radius: 20px;

<h3>Humedad</h3><div class="value"><span id="hum">--</span>%</div></div>            padding: 40px;

</div>            box-shadow: 0 20px 60px rgba(0,0,0,0.3);

<div id="chartContainer"><canvas id="chart"></canvas></div>            max-width: 500px;

<div style="text-align:center">            width: 100%;

<button class="btn" onclick="exportCSV()">📥 Exportar CSV</button>        }

<button class="btn" onclick="location.href='/metrics'">📊 Metrics</button>        

</div>        h1 {

<div class="status"><span class="dot"></span><strong id="status">Conectado</strong><div style="font-size:12px;color:#666;margin-top:4px" id="lastUpdate">Cargando...</div></div>            color: #333;

</div>            text-align: center;

<script>            margin-bottom: 10px;

const ctx=document.getElementById('chart').getContext('2d');            font-size: 28px;

let chart=new Chart(ctx,{type:'line',data:{labels:[],datasets:[{label:'Temp(°C)',data:[],borderColor:'#f5576c',fill:false},{label:'Hum(%)',data:[],borderColor:'#4facfe',fill:false}]},options:{responsive:true,scales:{y:{beginAtZero:false}}}});        }

function apply(d){        

document.getElementById('temp').textContent=d.temperature.toFixed(1);        .subtitle {

document.getElementById('hum').textContent=d.humidity.toFixed(1);            text-align: center;

document.getElementById('lastUpdate').textContent='Última: '+new Date(d.time_iso||Date.now()).toLocaleTimeString();            color: #666;

}            margin-bottom: 30px;

async function loadHistory(){            font-size: 14px;

const r=await fetch('/api/history');const h=await r.json();        }

chart.data.labels=h.map(x=>new Date(x.timestamp*1000).toLocaleTimeString());        

chart.data.datasets[0].data=h.map(x=>x.temp);        .sensor-card {

chart.data.datasets[1].data=h.map(x=>x.hum);            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);

chart.update();            border-radius: 15px;

}            padding: 30px;

try{const es=new EventSource('/events');es.addEventListener('update',e=>{try{apply(JSON.parse(e.data));loadHistory()}catch(_){}});es.onerror=()=>{document.getElementById('status').textContent='Desconectado'};}catch(_){}            margin-bottom: 20px;

async function poll(){try{const r=await fetch('/api/sensors');apply(await r.json());loadHistory()}catch(_){}}            color: white;

setInterval(poll,5000);poll();loadHistory();            position: relative;

function exportCSV(){window.open('/api/export','_blank')}            overflow: hidden;

</script>        }

</body>        

</html>        .sensor-card::before {

)rawliteral";            content: '';

            position: absolute;

// ==================== FUNCTIONS ====================            top: -50%;

void setupWiFiManager() {            right: -50%;

  WiFi.mode(WIFI_STA);            width: 200%;

  WiFiManager wm;            height: 200%;

  wm.setConfigPortalTimeout(180);            background: rgba(255,255,255,0.1);

  if(!wm.autoConnect("ESP32-TempMonitor")) {            border-radius: 50%;

    Serial.println("WiFi failed, restarting...");        }

    delay(3000); ESP.restart();        

  }        .sensor-label {

  Serial.println("✓ WiFi conectado: " + WiFi.localIP().toString());            font-size: 14px;

  digitalWrite(LED_PIN, HIGH);            opacity: 0.9;

            margin-bottom: 10px;

  if (MDNS.begin("esp32-temp")) {            text-transform: uppercase;

    MDNS.addService("http","tcp",80);            letter-spacing: 1px;

    Serial.println("✓ mDNS: http://esp32-temp.local");        }

  }        

  configTime(0,0,"pool.ntp.org");        .sensor-value {

}            font-size: 48px;

            font-weight: bold;

void setupOTA() {            position: relative;

  ArduinoOTA.setHostname("esp32-temp");            z-index: 1;

  ArduinoOTA.onStart([](){ Serial.println("OTA Start"); });        }

  ArduinoOTA.onEnd([](){ Serial.println("\nOTA End"); });        

  ArduinoOTA.onProgress([](unsigned int p, unsigned int t){        .sensor-unit {

    Serial.printf("OTA: %u%%\r",(p/(t/100)));            font-size: 24px;

  });            opacity: 0.9;

  ArduinoOTA.onError([](ota_error_t e){        }

    Serial.printf("OTA Error[%u]: ",e);        

  });        .temp-card {

  ArduinoOTA.begin();            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);

  Serial.println("✓ OTA ready");        }

}        

        .humidity-card {

void addReading(float t, float h) {            background: linear-gradient(135deg, #4facfe 0%, #00f2fe 100%);

  history[historyIndex].temp = t;        }

  history[historyIndex].hum = h;        

  history[historyIndex].timestamp = time(nullptr);        .status {

  historyIndex = (historyIndex + 1) % HISTORY_SIZE;            text-align: center;

  if (historyCount < HISTORY_SIZE) historyCount++;            padding: 15px;

}            background: #f0f0f0;

            border-radius: 10px;

void setupWebServer() {            margin-top: 20px;

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){ req->send_P(200,"text/html",index_html); });        }

        

  server.on("/api/sensors", HTTP_GET, [](AsyncWebServerRequest *req){        .status-dot {

    StaticJsonDocument<256> doc;            display: inline-block;

    doc["temperature"]=temperature; doc["humidity"]=humidity;            width: 10px;

    doc["timestamp"]=millis();            height: 10px;

    time_t now=time(nullptr); if(now>100000){char buf[32];strftime(buf,sizeof(buf),"%Y-%m-%dT%H:%M:%SZ",gmtime(&now));doc["time_iso"]=buf;}            border-radius: 50%;

    String out;serializeJson(doc,out);req->send(200,"application/json",out);            background: #4CAF50;

  });            margin-right: 8px;

            animation: pulse 2s infinite;

  server.on("/api/history", HTTP_GET, [](AsyncWebServerRequest *req){        }

    DynamicJsonDocument doc(4096);        

    JsonArray arr=doc.to<JsonArray>();        @keyframes pulse {

    for(int i=0;i<historyCount;i++){            0%, 100% { opacity: 1; }

      int idx=(historyIndex+HISTORY_SIZE-historyCount+i)%HISTORY_SIZE;            50% { opacity: 0.5; }

      JsonObject o=arr.createNestedObject();        }

      o["temp"]=history[idx].temp; o["hum"]=history[idx].hum; o["timestamp"]=history[idx].timestamp;        

    }        .last-update {

    String out;serializeJson(doc,out);req->send(200,"application/json",out);            color: #666;

  });            font-size: 12px;

            margin-top: 5px;

  server.on("/api/export", HTTP_GET, [](AsyncWebServerRequest *req){        }

    String csv="timestamp,temperature,humidity\n";        

    for(int i=0;i<historyCount;i++){        @media (max-width: 480px) {

      int idx=(historyIndex+HISTORY_SIZE-historyCount+i)%HISTORY_SIZE;            .container { padding: 20px; }

      csv+=String(history[idx].timestamp)+","+String(history[idx].temp,2)+","+String(history[idx].hum,2)+"\n";            h1 { font-size: 24px; }

    }            .sensor-value { font-size: 36px; }

    req->send(200,"text/csv",csv);        }

  });    </style>

</head>

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *req){<body>

    StaticJsonDocument<300> doc;    <div class="container">

    doc["wifi_ssid"]=WiFi.SSID();doc["wifi_rssi"]=WiFi.RSSI();doc["ip"]=WiFi.localIP().toString();doc["uptime_ms"]=millis();        <h1>🌡️ Monitor de Clima</h1>

    doc["free_heap"]=ESP.getFreeHeap();doc["chip_model"]=ESP.getChipModel();        <p class="subtitle">ESP32 IoT Dashboard</p>

    String out;serializeJson(doc,out);req->send(200,"application/json",out);        

  });        <div class="sensor-card temp-card">

            <div class="sensor-label">Temperatura</div>

  server.on("/metrics", HTTP_GET, [](AsyncWebServerRequest *req){            <div class="sensor-value">

    String m="esp32_temperature_celsius ";m+=String(temperature,2);m+="\nesp32_humidity_percent ";m+=String(humidity,2);m+="\n";                <span id="temperature">--</span><span class="sensor-unit">°C</span>

    req->send(200,"text/plain; version=0.0.4",m);            </div>

  });        </div>

        

  server.on("/healthz", HTTP_GET, [](AsyncWebServerRequest *req){ req->send(200,"text/plain","ok"); });        <div class="sensor-card humidity-card">

            <div class="sensor-label">Humedad</div>

  events.onConnect([](AsyncEventSourceClient *c){            <div class="sensor-value">

    StaticJsonDocument<200> doc;doc["temperature"]=temperature;doc["humidity"]=humidity;String p;serializeJson(doc,p);c->send(p.c_str(),"update",millis());                <span id="humidity">--</span><span class="sensor-unit">%</span>

  });            </div>

  server.addHandler(&events);        </div>

  server.onNotFound([](AsyncWebServerRequest *req){ req->send(404,"text/plain","404"); });        

  server.begin();        <div class="status">

  Serial.println("✓ Server on :80\n");            <span class="status-dot"></span>

}            <strong>Conectado</strong>

            <div class="last-update" id="lastUpdate">Actualizando...</div>

void readSensors() {        </div>

  unsigned long now=millis();    </div>

  if(now-lastReadTime<SENSOR_READ_INTERVAL) return;    

  lastReadTime=now;    <script>

  float t=dht.readTemperature(), h=dht.readHumidity();        const lastUpdate = document.getElementById('lastUpdate');

  if(isnan(t)||isnan(h)){ Serial.println("⚠️ DHT error"); return; }        function applyData(data){

  temperature=t; humidity=h;            document.getElementById('temperature').textContent = Number(data.temperature).toFixed(1);

  addReading(t,h);            document.getElementById('humidity').textContent = Number(data.humidity).toFixed(1);

  Serial.printf("🌡️ %.1f°C | 💧 %.1f%%\n",t,h);            const ts = data.time_iso || new Date().toISOString();

  StaticJsonDocument<200> doc;doc["temperature"]=t;doc["humidity"]=h;            const t = new Date(ts);

  time_t nt=time(nullptr);if(nt>100000){char buf[32];strftime(buf,sizeof(buf),"%Y-%m-%dT%H:%M:%SZ",gmtime(&nt));doc["time_iso"]=buf;}            lastUpdate.textContent = 'Última actualización: ' + t.toLocaleTimeString('es-ES');

  String payload;serializeJson(doc,payload);events.send(payload.c_str(),"update",millis());        }

}

        // Live updates via SSE with graceful fallback

// ==================== SETUP & LOOP ====================        try {

void setup() {            const es = new EventSource('/events');

  Serial.begin(115200); delay(1000);            es.addEventListener('update', (ev)=>{

  pinMode(LED_PIN,OUTPUT); digitalWrite(LED_PIN,LOW);                try { applyData(JSON.parse(ev.data)); } catch(e){}

  dht.begin();            });

  Serial.println("\n🌡️ ESP32 Temp Monitor PRO v2.0");            es.onerror = ()=>{ /* fallback below will keep trying */ };

  setupWiFiManager();        } catch(e) {}

  setupOTA();

  setupWebServer();        async function poll(){

  Serial.printf("📱 http://%s | http://esp32-temp.local\n",WiFi.localIP().toString().c_str());            try{

}                const r = await fetch('/api/sensors');

                applyData(await r.json());

void loop() {            }catch(e){ lastUpdate.textContent = 'Error de conexión'; }

  ArduinoOTA.handle();        }

  readSensors();        setInterval(poll, 5000);

  delay(100);        poll();

}    </script>

</body>
</html>
)rawliteral";

// ==================== FUNCIONES ====================

void setupWiFi() {
    Serial.println("\n========================================");
    Serial.println("  ESP32 Temperature Monitor v1.0");
    Serial.println("========================================\n");
    
    Serial.print("Conectando a WiFi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Parpadeo durante conexión
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✓ WiFi conectado exitosamente!");
        Serial.print("✓ Dirección IP: ");
        Serial.println(WiFi.localIP());
        Serial.print("✓ Señal WiFi: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm\n");
        digitalWrite(LED_PIN, HIGH); // LED encendido = conectado

        // mDNS
        if (MDNS.begin("esp32-temp")) {
            Serial.println("✓ mDNS: http://esp32-temp.local");
            MDNS.addService("http", "tcp", 80);
        } else {
            Serial.println("✗ mDNS no disponible");
        }

        // NTP (UTC)
        configTime(0, 0, "pool.ntp.org", "time.nist.gov");
        struct tm tm_info; time_t now = time(nullptr);
        if (now > 100000) { lastNtpSync = now; }
    } else {
        Serial.println("\n✗ Error: No se pudo conectar al WiFi");
        Serial.println("Verifica SSID y contraseña en el código\n");
        digitalWrite(LED_PIN, LOW);
    }
}

void setupWebServer() {
    // Ruta principal - Dashboard HTML
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });
    
    // API REST - Datos en formato JSON
    server.on("/api/sensors", HTTP_GET, [](AsyncWebServerRequest *request){
        StaticJsonDocument<200> doc;
        doc["temperature"] = temperature;
        doc["humidity"] = humidity;
        doc["timestamp"] = millis();
        time_t now = time(nullptr);
        if (now > 100000) {
            char buf[32];
            struct tm *t = gmtime(&now);
            strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", t);
            doc["time_iso"] = buf;
        }
        doc["unit_temp"] = "°C";
        doc["unit_humidity"] = "%";
        
        String response;
        serializeJson(doc, response);
        
        request->send(200, "application/json", response);
    });
    
    // Endpoint de estado del sistema
    server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
        StaticJsonDocument<300> doc;
        doc["wifi_ssid"] = WiFi.SSID();
        doc["wifi_rssi"] = WiFi.RSSI();
        doc["ip_address"] = WiFi.localIP().toString();
        doc["uptime_ms"] = millis();
        doc["free_heap"] = ESP.getFreeHeap();
        doc["chip_model"] = ESP.getChipModel();
        
        String response;
        serializeJson(doc, response);
        
        request->send(200, "application/json", response);
    });

    // Health check
    server.on("/healthz", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "ok");
    });

    // Prometheus metrics (simple)
    server.on("/metrics", HTTP_GET, [](AsyncWebServerRequest *request){
        String m;
        m += "esp32_temperature_celsius "; m += String(temperature, 2); m += "\n";
        m += "esp32_humidity_percent "; m += String(humidity, 2); m += "\n";
        request->send(200, "text/plain; version=0.0.4", m);
    });

    // SSE events
    events.onConnect([](AsyncEventSourceClient *client){
        if(client->lastId()) {
            Serial.printf("[SSE] Reconexion, id: %u\n", client->lastId());
        }
        // Send an initial snapshot
        StaticJsonDocument<200> doc;
        doc["temperature"] = temperature;
        doc["humidity"] = humidity;
        String payload; serializeJson(doc, payload);
        client->send(payload.c_str(), "update", millis());
    });
    server.addHandler(&events);
    
    // Manejo de rutas no encontradas
    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "404: Página no encontrada");
    });
    
    server.begin();
    Serial.println("✓ Servidor web iniciado en puerto 80");
    Serial.println("========================================\n");
}

void readSensors() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastReadTime >= SENSOR_READ_INTERVAL) {
        lastReadTime = currentTime;
        
        float newTemp = dht.readTemperature();
        float newHumidity = dht.readHumidity();
        
        if (!isnan(newTemp) && !isnan(newHumidity)) {
            temperature = newTemp;
            humidity = newHumidity;
            
            Serial.printf("🌡️  Temp: %.1f°C | 💧 Humedad: %.1f%%\n", temperature, humidity);

            // Emit SSE update
            StaticJsonDocument<200> doc;
            doc["temperature"] = temperature;
            doc["humidity"] = humidity;
            time_t now = time(nullptr);
            if (now > 100000) {
                char buf[32];
                struct tm *t = gmtime(&now);
                strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", t);
                doc["time_iso"] = buf;
            }
            String payload; serializeJson(doc, payload);
            events.send(payload.c_str(), "update", millis());
        } else {
            Serial.println("⚠️  Error leyendo sensor DHT22");
        }
    }
}

// ==================== SETUP Y LOOP ====================

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    dht.begin();
    
    setupWiFi();
    
    if (WiFi.status() == WL_CONNECTED) {
        setupWebServer();
        
        Serial.println("📱 Abre tu navegador y ve a:");
        Serial.print("   http://");
        Serial.println(WiFi.localIP());
        Serial.println("   http://esp32-temp.local (mDNS)");
        Serial.println("\n🔗 API Endpoints disponibles:");
        Serial.println("   /api/sensors  - Datos de temperatura y humedad");
        Serial.println("   /api/status   - Estado del sistema\n");
        Serial.println("   /events       - Server-Sent Events (tiempo real)");
        Serial.println("   /metrics      - Prometheus metrics");
        Serial.println("   /healthz      - Health check\n");
    }
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        readSensors();
    } else {
        Serial.println("⚠️  WiFi desconectado. Reintentando...");
        digitalWrite(LED_PIN, LOW);
        setupWiFi();
        delay(5000);
    }
    
    delay(100);
}
