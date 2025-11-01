# 🌡️ ESP32 Temperature & Humidity Monitor PRO v2.0# 🌡️ ESP32 Temperature & Humidity Monitor



Sistema IoT profesional de monitoreo ambiental con capacidades avanzadas de configuración, actualización remota y observabilidad completa.![ESP32](https://img.shields.io/badge/ESP32-DevKit-blue)

![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange)

![ESP32](https://img.shields.io/badge/ESP32-DevKit-blue)![License](https://img.shields.io/badge/License-MIT-green)

![PlatformIO](https://img.shields.io/badge/PlatformIO-Core-orange)

![License](https://img.shields.io/badge/license-MIT-green)Sistema IoT profesional para monitoreo de temperatura y humedad en tiempo real utilizando ESP32 y sensor DHT22, con dashboard web responsive y API REST.



## ✨ Features v2.0 PRO## 📸 Preview



### 🚀 Productividad y DevOps![Dashboard Preview](https://via.placeholder.com/800x400/667eea/ffffff?text=Dashboard+Preview)

- **OTA (Over-The-Air Updates)**: Actualiza firmware de forma remota vía ArduinoOTA sin cables

- **WiFiManager**: Portal cautivo para configuración WiFi (sin hardcodear credenciales)## ✨ Características

- **mDNS**: Acceso vía `http://esp32-temp.local` (sin recordar IP)

- 🌡️ **Monitoreo en Tiempo Real**: Lectura continua de temperatura y humedad

### 📊 Observabilidad y Datos- 📱 **Dashboard Web Responsive**: Interfaz moderna y adaptable a cualquier dispositivo

- **Dashboard Web Interactivo**: UI moderna con Chart.js para gráficos en tiempo real- 🔌 **API REST**: Endpoints JSON para integración con otros sistemas

- **Historial de Lecturas**: Buffer de últimas 50 mediciones con timestamps NTP- ⚡ **Tiempo Real (SSE)**: Actualizaciones instantáneas con Server‑Sent Events

- **Server-Sent Events (SSE)**: Actualizaciones en tiempo real sin polling- 🌐 **mDNS**: Accede con `http://esp32-temp.local`

- **Export CSV**: Descarga histórico completo para análisis externo- 🕒 **NTP**: Timestamps en UTC (`time_iso`) vía NTP

- **Prometheus Metrics**: Endpoint `/metrics` para integración con Prometheus/Grafana- 🔔 **Indicador de Estado**: LED integrado muestra estado de conexión WiFi

- ⚡ **Bajo Consumo**: Optimizado para eficiencia energética

### 🔧 Endpoints API REST- 🎨 **Diseño Moderno**: UI con gradientes y animaciones suaves

- `GET /` - Dashboard principal con gráficos Chart.js

- `GET /api/sensors` - Lectura actual (JSON)## 🛠️ Hardware Requerido

- `GET /api/history` - Historial completo (JSON)

- `GET /api/export` - Export CSV de histórico| Componente | Cantidad | Notas |

- `GET /api/status` - Estado del sistema (WiFi, heap, uptime)|------------|----------|-------|

- `GET /metrics` - Métricas Prometheus| ESP32 DevKit | 1 | Cualquier versión compatible |

- `GET /healthz` - Health check| Sensor DHT22 | 1 | Sensor de temperatura y humedad |

- `GET /events` - SSE stream| Cables Jumper | 3 | Macho-Hembra |

| Protoboard | 1 | Opcional para prototipado |

## 🛠️ Hardware Requerido

## 📐 Diagrama de Conexión

| Componente | Descripción | GPIO |

|------------|-------------|------|```

| ESP32 DevKit | Microcontrolador principal | - |ESP32           DHT22

| DHT22 | Sensor temperatura/humedad | GPIO 4 |-----           -----

| LED (opcional) | Indicador de estado | GPIO 2 |3.3V    <--->   VCC

GND     <--->   GND

### 📐 ConexionesGPIO 4  <--->   DATA

```

```

DHT22:### Pines Utilizados

  VCC  -> 3.3V

  DATA -> GPIO 4- **GPIO 4**: Señal de datos DHT22

  GND  -> GND- **GPIO 2**: LED indicador de estado (LED integrado)



LED (integrado):## 🚀 Instalación

  GPIO 2 (Built-in LED)

```### Requisitos Previos



## 📦 Instalación- [Visual Studio Code](https://code.visualstudio.com/)

- [PlatformIO Extension](https://platformio.org/install/ide?install=vscode)

### 1. Clonar repositorio- Cable USB para ESP32

```bash

git clone https://github.com/SEBASTIAN3451/ESP32-Temperature-Monitor-.git### Pasos de Instalación

cd ESP32-Temperature-Monitor-

```1. **Clonar el repositorio**

   ```bash

### 2. Instalar PlatformIO   git clone https://github.com/SEBASTIAN3451/ESP32-Temperature-Monitor-.git

```bash   cd ESP32-Temperature-Monitor-

pip install platformio   ```

```

2. **Abrir en VS Code**

### 3. Compilar y cargar   ```bash

```bash   code .

pio run -t upload   ```

```

3. **Configurar WiFi**

### 4. Configuración WiFi (Primera vez)   

1. Al encender, el ESP32 crea red `ESP32-TempMonitor`   Edita el archivo `src/main.cpp` y cambia estas líneas:

2. Conectar desde smartphone/PC   ```cpp

3. Se abre portal cautivo automáticamente   const char* ssid = "TU_WIFI_SSID";

4. Seleccionar tu WiFi e ingresar contraseña   const char* password = "TU_WIFI_PASSWORD";

5. ¡Listo! Las credenciales se guardan permanentemente   ```



## 🚀 Uso4. **Compilar y Subir**

   

### Dashboard Web   En PlatformIO:

- Acceder a: `http://esp32-temp.local` (o IP mostrada en Serial Monitor)   - Click en el ícono ✓ (Build) para compilar

- Ver temperatura/humedad en tiempo real   - Click en → (Upload) para subir al ESP32

- Gráfico histórico interactivo (Chart.js)   - Click en 🔌 (Serial Monitor) para ver el output

- Exportar datos CSV

## 📡 Uso

### Actualización OTA

```bash### Acceder al Dashboard

# Desde PlatformIO

pio run -t upload --upload-port esp32-temp.local1. Conecta tu ESP32 y observa el Monitor Serial

2. Anota la dirección IP mostrada (ej: `192.168.1.100`)

# O desde Arduino IDE:3. Abre tu navegador y ve a: `http://192.168.1.100`

# Tools > Port > esp32-temp at <IP> (OTA)

```### API Endpoints



### Prometheus Integration#### `GET /api/sensors`

Agregar a `prometheus.yml`:Obtiene los datos actuales de temperatura y humedad

```yaml

scrape_configs:**Respuesta:**

  - job_name: 'esp32-temp'```json

    static_configs:{

      - targets: ['esp32-temp.local:80']  "temperature": 23.5,

    metrics_path: '/metrics'  "humidity": 65.2,

```  "timestamp": 12345678,

   "time_iso": "2025-10-31T20:15:10Z",

## 📡 API Examples  "unit_temp": "°C",

  "unit_humidity": "%"

### Lectura actual}

```bash```

curl http://esp32-temp.local/api/sensors

# {"temperature":23.5,"humidity":65.2,"timestamp":12345,"time_iso":"2025-10-31T10:30:00Z"}#### `GET /api/status`

```Obtiene el estado del sistema



### Histórico completo**Respuesta:**

```bash```json

curl http://esp32-temp.local/api/history{

# [{"temp":23.1,"hum":64.5,"timestamp":1730000000}, ...]  "wifi_ssid": "MiWiFi",

```  "wifi_rssi": -45,

  "ip_address": "192.168.1.100",

### Export CSV  "uptime_ms": 3600000,

```bash  "free_heap": 256000,

curl http://esp32-temp.local/api/export -o data.csv  "chip_model": "ESP32-D0WDQ6"

```}



### SSE Stream (JS)#### `GET /events`

```javascriptServer‑Sent Events con actualizaciones en tiempo real (evento "update").

const es = new EventSource('http://esp32-temp.local/events');

es.addEventListener('update', (e) => {#### `GET /metrics`

  const data = JSON.parse(e.data);Métricas en formato Prometheus (`esp32_temperature_celsius`, `esp32_humidity_percent`).

  console.log(`Temp: ${data.temperature}°C | Hum: ${data.humidity}%`);

});#### `GET /healthz`

```Health check simple.

```

## 📊 Screenshots

## 📊 Características Técnicas

### Dashboard Principal

Dashboard moderno con:| Característica | Especificación |

- Cards de temperatura y humedad|----------------|----------------|

- Gráfico Chart.js interactivo (líneas dual-axis)| Microcontrolador | ESP32 (Dual-Core 240MHz) |

- Botones de exportación| Conectividad | WiFi 802.11 b/g/n |

- Indicador de conexión en tiempo real| Sensor | DHT22 (AM2302) |

| Rango Temperatura | -40°C a 80°C (±0.5°C) |

### Prometheus Metrics| Rango Humedad | 0% a 100% (±2%) |

```| Frecuencia Lectura | 2 segundos |

esp32_temperature_celsius 23.50| Servidor Web | AsyncWebServer (80) |

esp32_humidity_percent 65.20| Consumo | ~80mA (típico) |

```

## 🔧 Personalización

## 🔧 Configuración Avanzada

### Cambiar Intervalo de Lectura

### Cambiar umbrales o GPIOs

Editar `src/main.cpp`:En `src/main.cpp`:

```cpp```cpp

#define DHT_PIN 4           // GPIO del DHT22const unsigned long SENSOR_READ_INTERVAL = 2000; // milisegundos

#define SENSOR_READ_INTERVAL 2000  // ms entre lecturas```

#define HISTORY_SIZE 50     // Tamaño del buffer

```### Cambiar Pines



### Reset WiFiEn `src/main.cpp`:

Mantener presionado botón BOOT al encender para entrar en modo configuración.```cpp

#define DHT_PIN 4    // Pin del sensor

## 🧪 Troubleshooting#define LED_PIN 2    // Pin del LED

```

| Problema | Solución |

|----------|----------|### Modificar el Dashboard

| No aparece red WiFi | Presionar BOOT al encender, esperar 10s |

| OTA no funciona | Verificar firewall, usar IP directa |El HTML está embebido en `src/main.cpp` en la variable `index_html`. Puedes editar:

| DHT lee NaN | Verificar conexiones, resistor pull-up 10kΩ |- Colores (gradientes CSS)

| mDNS no resuelve | En Windows instalar Bonjour Print Services |- Texto e idioma

- Frecuencia de actualización JavaScript

## 📚 Stack Tecnológico

## 🐛 Troubleshooting

- **Framework**: Arduino (ESP32)

- **Build System**: PlatformIO### El ESP32 no se conecta al WiFi

- **Web Server**: ESPAsyncWebServer (async/non-blocking)

- **JSON**: ArduinoJson v6- ✓ Verifica que el SSID y contraseña sean correctos

- **Charts**: Chart.js 4.4- ✓ Asegúrate de estar en red 2.4GHz (ESP32 no soporta 5GHz)

- **WiFi Config**: WiFiManager (tzapu)- ✓ Verifica que el router esté encendido y en rango

- **OTA**: ArduinoOTA

- **Time Sync**: NTP (pool.ntp.org)### El sensor no lee valores

- **mDNS**: ESP mDNS

- ✓ Verifica las conexiones (VCC, GND, DATA)

## 📁 Estructura del Proyecto- ✓ Asegúrate de usar GPIO 4 o cambia el pin en el código

- ✓ Espera 2-3 segundos después del inicio

```

ESP32_Temperature_Monitor/### La página web no carga

├── src/

│   ├── main.cpp              # Código principal v2.0 PRO- ✓ Verifica la IP en el Monitor Serial

│   └── main_backup_v1.cpp    # Backup versión 1.0- ✓ Asegúrate de estar en la misma red WiFi

├── platformio.ini            # Configuración PlatformIO- ✓ Intenta reiniciar el ESP32

└── README.md

```## 📚 Librerías Utilizadas



## 🔄 Changelog- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library) - Lectura de sensor DHT

- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor) - Abstracción de sensores

### v2.0 PRO (Octubre 2025)- [ArduinoJson](https://arduinojson.org/) - Serialización JSON

- ✅ OTA (ArduinoOTA) para updates remotos- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) - Servidor web asíncrono

- ✅ WiFiManager para config sin hardcode

- ✅ Historial 50 lecturas con timestamps NTP## 🤝 Contribuciones

- ✅ Chart.js 4.4 embedded en dashboard

- ✅ Export CSV de históricoLas contribuciones son bienvenidas! Por favor:

- ✅ Prometheus metrics endpoint

- ✅ mDNS (esp32-temp.local)1. Fork el proyecto

- ✅ SSE para tiempo real2. Crea una rama para tu feature (`git checkout -b feature/NuevaCaracteristica`)

- ✅ Endpoints REST completos3. Commit tus cambios (`git commit -m 'Agregar nueva característica'`)

4. Push a la rama (`git push origin feature/NuevaCaracteristica`)

### v1.0 (Inicial)5. Abre un Pull Request

- Monitoreo básico DHT22

- Dashboard web simple## 📝 Licencia

- SSE updates

Este proyecto está bajo la Licencia MIT. Ver archivo `LICENSE` para más detalles.

## 🤝 Contribuciones

## 👤 Autor

¡Pull requests bienvenidos! Para cambios mayores, abrir issue primero.

**Sebastian Lara**

## 📄 Licencia

- GitHub: [@SEBASTIAN3451](https://github.com/SEBASTIAN3451)

MIT License - Ver archivo LICENSE- LinkedIn: [Tu LinkedIn](https://linkedin.com/in/tu-perfil)



## 👨‍💻 Autor## 🌟 Proyectos Relacionados



**Sebastian Lara**- [ESP32 Smart Irrigation](https://github.com/SEBASTIAN3451/ESP32-Smart-Irrigation-)

- GitHub: [@SEBASTIAN3451](https://github.com/SEBASTIAN3451)- [ESP32 Weather Station](https://github.com/SEBASTIAN3451/ESP32-Weather-Station)

- Proyecto: [ESP32-Temperature-Monitor-](https://github.com/SEBASTIAN3451/ESP32-Temperature-Monitor-)

## 📞 Soporte

---

Si tienes alguna pregunta o problema, abre un [Issue](https://github.com/SEBASTIAN3451/ESP32-Temperature-Monitor-/issues).

⭐ Si este proyecto te fue útil, dale una estrella en GitHub!

---

⭐ Si este proyecto te fue útil, dale una estrella en GitHub!
