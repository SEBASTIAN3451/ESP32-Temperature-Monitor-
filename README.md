# 🌡️ ESP32 Temperature & Humidity Monitor

![ESP32](https://img.shields.io/badge/ESP32-DevKit-blue)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange)
![License](https://img.shields.io/badge/License-MIT-green)

Sistema IoT profesional para monitoreo de temperatura y humedad en tiempo real utilizando ESP32 y sensor DHT22, con dashboard web responsive y API REST.

## 📸 Preview

![Dashboard Preview](https://via.placeholder.com/800x400/667eea/ffffff?text=Dashboard+Preview)

## ✨ Características

- 🌡️ **Monitoreo en Tiempo Real**: Lectura continua de temperatura y humedad
- 📱 **Dashboard Web Responsive**: Interfaz moderna y adaptable a cualquier dispositivo
- 🔌 **API REST**: Endpoints JSON para integración con otros sistemas
- ⚡ **Tiempo Real (SSE)**: Actualizaciones instantáneas con Server‑Sent Events
- 🌐 **mDNS**: Accede con `http://esp32-temp.local`
- 🕒 **NTP**: Timestamps en UTC (`time_iso`) vía NTP
- 🔔 **Indicador de Estado**: LED integrado muestra estado de conexión WiFi
- ⚡ **Bajo Consumo**: Optimizado para eficiencia energética
- 🎨 **Diseño Moderno**: UI con gradientes y animaciones suaves

## 🛠️ Hardware Requerido

| Componente | Cantidad | Notas |
|------------|----------|-------|
| ESP32 DevKit | 1 | Cualquier versión compatible |
| Sensor DHT22 | 1 | Sensor de temperatura y humedad |
| Cables Jumper | 3 | Macho-Hembra |
| Protoboard | 1 | Opcional para prototipado |

## 📐 Diagrama de Conexión

```
ESP32           DHT22
-----           -----
3.3V    <--->   VCC
GND     <--->   GND
GPIO 4  <--->   DATA
```

### Pines Utilizados

- **GPIO 4**: Señal de datos DHT22
- **GPIO 2**: LED indicador de estado (LED integrado)

## 🚀 Instalación

### Requisitos Previos

- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO Extension](https://platformio.org/install/ide?install=vscode)
- Cable USB para ESP32

### Pasos de Instalación

1. **Clonar el repositorio**
   ```bash
   git clone https://github.com/SEBASTIAN3451/ESP32-Temperature-Monitor-.git
   cd ESP32-Temperature-Monitor-
   ```

2. **Abrir en VS Code**
   ```bash
   code .
   ```

3. **Configurar WiFi**
   
   Edita el archivo `src/main.cpp` y cambia estas líneas:
   ```cpp
   const char* ssid = "TU_WIFI_SSID";
   const char* password = "TU_WIFI_PASSWORD";
   ```

4. **Compilar y Subir**
   
   En PlatformIO:
   - Click en el ícono ✓ (Build) para compilar
   - Click en → (Upload) para subir al ESP32
   - Click en 🔌 (Serial Monitor) para ver el output

## 📡 Uso

### Acceder al Dashboard

1. Conecta tu ESP32 y observa el Monitor Serial
2. Anota la dirección IP mostrada (ej: `192.168.1.100`)
3. Abre tu navegador y ve a: `http://192.168.1.100`

### API Endpoints

#### `GET /api/sensors`
Obtiene los datos actuales de temperatura y humedad

**Respuesta:**
```json
{
  "temperature": 23.5,
  "humidity": 65.2,
  "timestamp": 12345678,
   "time_iso": "2025-10-31T20:15:10Z",
  "unit_temp": "°C",
  "unit_humidity": "%"
}
```

#### `GET /api/status`
Obtiene el estado del sistema

**Respuesta:**
```json
{
  "wifi_ssid": "MiWiFi",
  "wifi_rssi": -45,
  "ip_address": "192.168.1.100",
  "uptime_ms": 3600000,
  "free_heap": 256000,
  "chip_model": "ESP32-D0WDQ6"
}

#### `GET /events`
Server‑Sent Events con actualizaciones en tiempo real (evento "update").

#### `GET /metrics`
Métricas en formato Prometheus (`esp32_temperature_celsius`, `esp32_humidity_percent`).

#### `GET /healthz`
Health check simple.
```

## 📊 Características Técnicas

| Característica | Especificación |
|----------------|----------------|
| Microcontrolador | ESP32 (Dual-Core 240MHz) |
| Conectividad | WiFi 802.11 b/g/n |
| Sensor | DHT22 (AM2302) |
| Rango Temperatura | -40°C a 80°C (±0.5°C) |
| Rango Humedad | 0% a 100% (±2%) |
| Frecuencia Lectura | 2 segundos |
| Servidor Web | AsyncWebServer (80) |
| Consumo | ~80mA (típico) |

## 🔧 Personalización

### Cambiar Intervalo de Lectura

En `src/main.cpp`:
```cpp
const unsigned long SENSOR_READ_INTERVAL = 2000; // milisegundos
```

### Cambiar Pines

En `src/main.cpp`:
```cpp
#define DHT_PIN 4    // Pin del sensor
#define LED_PIN 2    // Pin del LED
```

### Modificar el Dashboard

El HTML está embebido en `src/main.cpp` en la variable `index_html`. Puedes editar:
- Colores (gradientes CSS)
- Texto e idioma
- Frecuencia de actualización JavaScript

## 🐛 Troubleshooting

### El ESP32 no se conecta al WiFi

- ✓ Verifica que el SSID y contraseña sean correctos
- ✓ Asegúrate de estar en red 2.4GHz (ESP32 no soporta 5GHz)
- ✓ Verifica que el router esté encendido y en rango

### El sensor no lee valores

- ✓ Verifica las conexiones (VCC, GND, DATA)
- ✓ Asegúrate de usar GPIO 4 o cambia el pin en el código
- ✓ Espera 2-3 segundos después del inicio

### La página web no carga

- ✓ Verifica la IP en el Monitor Serial
- ✓ Asegúrate de estar en la misma red WiFi
- ✓ Intenta reiniciar el ESP32

## 📚 Librerías Utilizadas

- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library) - Lectura de sensor DHT
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor) - Abstracción de sensores
- [ArduinoJson](https://arduinojson.org/) - Serialización JSON
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) - Servidor web asíncrono

## 🤝 Contribuciones

Las contribuciones son bienvenidas! Por favor:

1. Fork el proyecto
2. Crea una rama para tu feature (`git checkout -b feature/NuevaCaracteristica`)
3. Commit tus cambios (`git commit -m 'Agregar nueva característica'`)
4. Push a la rama (`git push origin feature/NuevaCaracteristica`)
5. Abre un Pull Request

## 📝 Licencia

Este proyecto está bajo la Licencia MIT. Ver archivo `LICENSE` para más detalles.

## 👤 Autor

**Sebastian Lara**

- GitHub: [@SEBASTIAN3451](https://github.com/SEBASTIAN3451)
- LinkedIn: [Tu LinkedIn](https://linkedin.com/in/tu-perfil)

## 🌟 Proyectos Relacionados

- [ESP32 Smart Irrigation](https://github.com/SEBASTIAN3451/ESP32-Smart-Irrigation-)
- [ESP32 Weather Station](https://github.com/SEBASTIAN3451/ESP32-Weather-Station)

## 📞 Soporte

Si tienes alguna pregunta o problema, abre un [Issue](https://github.com/SEBASTIAN3451/ESP32-Temperature-Monitor-/issues).

---

⭐ Si este proyecto te fue útil, dale una estrella en GitHub!
