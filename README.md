# CommandInWiFi (CIW) – PoC sobre ESP32

## Descripción

Este repositorio lo hice porque necesitaba una prueba de concepto (PoC) de la vulnerabilidad conocida como **CommandInWiFi (CIW)**, basada en el procesamiento inseguro de SSIDs WiFi por parte de dispositivos que realizan escaneo pasivo.

Sirve demostrar cómo un dispositivo IoT puede verse afectado **sin necesidad de conexión explícita a una red**, únicamente mediante la recepción y procesamiento de nombres de red (SSID) especialmente construidos.

La PoC está implementada sobre ESP32 utilizando Arduino IDE.

---

## Vulnerabilidad: CommandInWiFi (CIW)

CommandInWiFi describe una clase de vulnerabilidades donde:

* El dispositivo escanea redes WiFi cercanas
* Procesa los SSIDs como entrada confiable
* No valida ni sanitiza correctamente dicha entrada

Esto permite a un atacante emitir SSIDs maliciosos que pueden provocar:

* Corrupción de memoria
* Fallos de ejecución
* Denegación de servicio (DoS)
* Potencial ejecución de código en entornos sin protecciones

Característica clave:

> El ataque es **zero-click**: no requiere interacción del usuario ni conexión a la red.

---

## PoC implementada

La prueba de concepto consiste en un firmware para ESP32 que:

1. Escanea redes WiFi periódicamente
2. Recupera los SSIDs detectados
3. Los procesa utilizando funciones inseguras (`snprintf` con formato controlado por entrada externa)

Esto introduce una vulnerabilidad clásica de tipo **format string**, permitiendo comportamiento indefinido cuando el SSID contiene especificadores de formato.

---

## Ejemplo de payload

SSID malicioso utilizado:

```text
%s%s%s%sAAAAAAAAAAAAAAAAAAAAAAAA
```

---

## Resultado observado

Al detectar el SSID anterior, el dispositivo entra en estado de fallo:

```text
SSID detectado: %s%s%s%sAAAAAAAAAAAAAAAAAAAAAAAA
Guru Meditation Error: Core  1 panic'ed (LoadProhibited). Exception was unhandled.
```

### Detalle técnico

* Tipo de excepción: `LoadProhibited`
* Causa: acceso a dirección inválida (`EXCVADDR: 0x0000ff00`)
* Origen: uso de punteros inválidos derivados de la interpretación del SSID como formato

Registro parcial:

```text
EXCCAUSE: 0x0000001c  
EXCVADDR: 0x0000ff00  
```

Backtrace:

```text
Backtrace: 0x4008919a:0x3ffb1d30 0x401590a2:0x3ffb1d40 ...
```

El dispositivo reinicia automáticamente tras el fallo:

```text
rst:0xc (SW_CPU_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
```

---

## Impacto

Esta PoC demuestra:

* Procesamiento inseguro de entrada externa (SSID)
* Vulnerabilidad de tipo format string
* Corrupción de memoria en tiempo de ejecución
* Denegación de servicio reproducible

En entornos sin protecciones de compilador o con manejo distinto de memoria, este tipo de vulnerabilidad podría escalar a escenarios más severos.

---

## Consideraciones

* Este código es únicamente para fines educativos y de investigación
* Debe ejecutarse en entornos controlados
* No se recomienda su uso fuera de laboratorio

La vulnerabilidad CommandInWiFi expone una superficie de ataque poco considerada: el procesamiento de SSIDs como entrada de datos.

Esta PoC evidencia que incluso operaciones aparentemente inocuas como el escaneo de redes pueden convertirse en vectores de ataque cuando no se aplican prácticas básicas de validación y sanitización.

Atacando desde https://github.com/7h30th3r0n3/Evil-M5Project/wiki/ciwzeroclick
y desde https://github.com/V33RU/CommandInWiFi-Zeroclick

---

## Licencia

Uso educativo y de investigación.
