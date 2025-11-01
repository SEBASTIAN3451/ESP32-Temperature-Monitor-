/*
 * ESP32 Temperature & Humidity Monitor
 * 
 * Sistema IoT para monitoreo de temperatura y humedad en tiempo real
 * con servidor web integrado y API REST.
 * 
 * Características:
 * - Lectura de sensor DHT22 (temperatura y humedad)
 * - Servidor web con dashboard responsive
 * - API REST para obtener datos en formato JSON
 * - Actualización automática cada 2 segundos
 * - Indicador LED para estado de conexión WiFi
 * 
 * Hardware:
 * - ESP32 DevKit
 * - Sensor DHT22
 * - LED (opcional)
 * 
 * Conexiones:
 * - DHT22 Data Pin -> GPIO 4
 * - LED -> GPIO 2 (LED integrado)
 * 
 * Autor: Sebastian Lara
 * Fecha: Octubre 2025
 */

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <time.h>
#include <DHT.h>
#include <ArduinoJson.h>

// ==================== CONFIGURACIÓN ====================

// Configuración WiFi - CAMBIAR ESTOS VALORES
const char* ssid = "TU_WIFI_SSID";
const char* password = "TU_WIFI_PASSWORD";

// Pines
#define DHT_PIN 4          // Pin del sensor DHT22
#define LED_PIN 2          // LED de estado (LED integrado del ESP32)
#define DHT_TYPE DHT22     // Tipo de sensor DHT

// Intervalo de lectura del sensor (milisegundos)
const unsigned long SENSOR_READ_INTERVAL = 2000;

// ==================== OBJETOS GLOBALES ====================

DHT dht(DHT_PIN, DHT_TYPE);
AsyncWebServer server(80);
AsyncEventSource events("/events");

// Variables para almacenar lecturas
float temperature = 0.0;
float humidity = 0.0;
unsigned long lastReadTime = 0;
time_t lastNtpSync = 0;

// ==================== PÁGINA WEB ====================

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Monitor de Temperatura ESP32</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }
        
        .container {
            background: white;
            border-radius: 20px;
            padding: 40px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            max-width: 500px;
            width: 100%;
        }
        
        h1 {
            color: #333;
            text-align: center;
            margin-bottom: 10px;
            font-size: 28px;
        }
        
        .subtitle {
            text-align: center;
            color: #666;
            margin-bottom: 30px;
            font-size: 14px;
        }
        
        .sensor-card {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            border-radius: 15px;
            padding: 30px;
            margin-bottom: 20px;
            color: white;
            position: relative;
            overflow: hidden;
        }
        
        .sensor-card::before {
            content: '';
            position: absolute;
            top: -50%;
            right: -50%;
            width: 200%;
            height: 200%;
            background: rgba(255,255,255,0.1);
            border-radius: 50%;
        }
        
        .sensor-label {
            font-size: 14px;
            opacity: 0.9;
            margin-bottom: 10px;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        
        .sensor-value {
            font-size: 48px;
            font-weight: bold;
            position: relative;
            z-index: 1;
        }
        
        .sensor-unit {
            font-size: 24px;
            opacity: 0.9;
        }
        
        .temp-card {
            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
        }
        
        .humidity-card {
            background: linear-gradient(135deg, #4facfe 0%, #00f2fe 100%);
        }
        
        .status {
            text-align: center;
            padding: 15px;
            background: #f0f0f0;
            border-radius: 10px;
            margin-top: 20px;
        }
        
        .status-dot {
            display: inline-block;
            width: 10px;
            height: 10px;
            border-radius: 50%;
            background: #4CAF50;
            margin-right: 8px;
            animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
        }
        
        .last-update {
            color: #666;
            font-size: 12px;
            margin-top: 5px;
        }
        
        @media (max-width: 480px) {
            .container { padding: 20px; }
            h1 { font-size: 24px; }
            .sensor-value { font-size: 36px; }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🌡️ Monitor de Clima</h1>
        <p class="subtitle">ESP32 IoT Dashboard</p>
        
        <div class="sensor-card temp-card">
            <div class="sensor-label">Temperatura</div>
            <div class="sensor-value">
                <span id="temperature">--</span><span class="sensor-unit">°C</span>
            </div>
        </div>
        
        <div class="sensor-card humidity-card">
            <div class="sensor-label">Humedad</div>
            <div class="sensor-value">
                <span id="humidity">--</span><span class="sensor-unit">%</span>
            </div>
        </div>
        
        <div class="status">
            <span class="status-dot"></span>
            <strong>Conectado</strong>
            <div class="last-update" id="lastUpdate">Actualizando...</div>
        </div>
    </div>
    
    <script>
        const lastUpdate = document.getElementById('lastUpdate');
        function applyData(data){
            document.getElementById('temperature').textContent = Number(data.temperature).toFixed(1);
            document.getElementById('humidity').textContent = Number(data.humidity).toFixed(1);
            const ts = data.time_iso || new Date().toISOString();
            const t = new Date(ts);
            lastUpdate.textContent = 'Última actualización: ' + t.toLocaleTimeString('es-ES');
        }

        // Live updates via SSE with graceful fallback
        try {
            const es = new EventSource('/events');
            es.addEventListener('update', (ev)=>{
                try { applyData(JSON.parse(ev.data)); } catch(e){}
            });
            es.onerror = ()=>{ /* fallback below will keep trying */ };
        } catch(e) {}

        async function poll(){
            try{
                const r = await fetch('/api/sensors');
                applyData(await r.json());
            }catch(e){ lastUpdate.textContent = 'Error de conexión'; }
        }
        setInterval(poll, 5000);
        poll();
    </script>
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
