# S-YL-69 — Sensor de Humedad del Suelo con ESP32

Sketch base para leer el sensor **YL-69 (FC-28)** desde un **ESP32** usando Arduino IDE. Muestra el valor ADC crudo, el porcentaje de humedad del suelo y el estado digital del módulo por el Serial Monitor.

---

## Tabla de contenidos

- [Descripción del sensor](#descripción-del-sensor)
- [Hardware requerido](#hardware-requerido)
- [Diagrama de conexión](#diagrama-de-conexión)
- [Conexión de pines](#conexión-de-pines)
- [Calibración](#calibración)
- [Configuración del sketch](#configuración-del-sketch)
- [Salida por Serial Monitor](#salida-por-serial-monitor)
- [Niveles de humedad](#niveles-de-humedad)
- [Recomendaciones de uso](#recomendaciones-de-uso)
- [Repositorio](#repositorio)

---

## Descripción del sensor

El **YL-69** es un sensor resistivo de humedad del suelo compuesto por dos partes:

- **Sonda FC-28**: dos electrodos de acero que se insertan en el suelo. Su resistencia varía inversamente con el contenido de agua.
- **Módulo comparador LM393**: convierte la resistencia en señal analógica (AO) y digital (DO) con umbral ajustable mediante potenciómetro.

| Parámetro | Valor |
|---|---|
| Voltaje de operación | 3.3 V – 5 V |
| Salida analógica (AO) | Variable — ADC alto = seco, ADC bajo = húmedo |
| Salida digital (DO) | HIGH = seco / LOW = húmedo (umbral ajustable) |
| Resolución ADC (ESP32) | 12 bits → 0–4095 |
| Librería requerida | Ninguna |

> **Lógica invertida**: a mayor ADC → mayor resistencia → suelo más seco.

---

## Hardware requerido

- ESP32 (cualquier variante)
- Módulo YL-69 con sonda FC-28
- Fuente de alimentación 3.3 V o 5 V
- Cables de conexión

---

## Diagrama de conexión

![Diagrama de conexión YL-69](https://hk.botsheet.com/wp-content/uploads/2021/04/soil-moisture-sensor-yl-69-wiring-1.jpeg?v=1657914282)

> La imagen muestra la conexión general del módulo YL-69. Para este proyecto los pines de datos van a los **GPIO 34 (analógico) y GPIO 23 (digital)** del ESP32.

---

## Conexión de pines

| Pin Módulo | Señal | ESP32 |
|---|---|---|
| `VCC` | Alimentación | 3.3 V o VIN (5 V) |
| `GND` | Tierra | GND |
| `AO` | Salida analógica | GPIO 34 (ADC1) |
| `DO` | Salida digital | GPIO 23 |

---

## Calibración

El sensor necesita dos valores de referencia para calcular el porcentaje de humedad:

| Constante | Procedimiento | Valor típico (ESP32 3.3V) |
|---|---|---|
| `VALOR_SECO` | Leer ADC con la sonda en el **aire** | 3200 – 3500 |
| `VALOR_HUMEDO` | Leer ADC con la sonda **sumergida en agua** | 800 – 1500 |

**Pasos:**
1. Cargar el sketch sin modificar.
2. Abrir el Serial Monitor y anotar el valor de `ADC (raw)` con la sonda en el aire → ese es `VALOR_SECO`.
3. Sumergir la sonda en un vaso de agua y anotar el valor → ese es `VALOR_HUMEDO`.
4. Actualizar las constantes en el sketch y recargar.

> Los valores variarán según el tipo de suelo, minerales presentes y voltaje de alimentación.

---

## Configuración del sketch

```cpp
#define PIN_ANALOGICO  34      // GPIO AO del módulo (ADC1, solo lectura)
#define PIN_DIGITAL    23      // GPIO DO del módulo

#define VALOR_SECO    3200     // ADC en aire (suelo completamente seco)
#define VALOR_HUMEDO   800     // ADC en agua (suelo completamente empapado)
```

---

## Salida por Serial Monitor

Abrir el Serial Monitor a **115200 bps**. Salida esperada:

```
=== YL-69 en ESP32 — Humedad del Suelo ===
----------------------------------
ADC (raw)     : 2850
Humedad       : 22 %
Estado        : SECO
Digital DO    : SECO
----------------------------------
ADC (raw)     : 1120
Humedad       : 87 %
Estado        : MUY HUMEDO
Digital DO    : HUMEDO (umbral superado)
----------------------------------
```

---

## Niveles de humedad

| Porcentaje | Nivel | Descripción |
|---|---|---|
| 80 – 100 % | MUY HUMEDO | Suelo saturado, posible encharcamiento |
| 60 – 79 % | HUMEDO | Suelo con buena cantidad de agua |
| 40 – 59 % | OPTIMO | Rango ideal para la mayoría de plantas |
| 20 – 39 % | SECO | Suelo con poca agua, considerar riego |
| 0 – 19 % | MUY SECO | Suelo árido, riego urgente |

---

## Recomendaciones de uso

- **Corrosión**: el sensor se degrada si permanece continuamente energizado en el suelo. Para extender su vida útil, alimentar el VCC desde un GPIO digital y activarlo solo durante la lectura.
- **Tipo de suelo**: los valores ADC varían con arena, arcilla o tierra compuesta. Calibrar siempre con el suelo real del proyecto.
- **Frecuencia de lectura**: en proyectos de riego, leer una o dos veces al día es suficiente. Evitar lecturas continuas.

---

## Repositorio

[https://github.com/SIGMA-SV4/S-YL-69](https://github.com/SIGMA-SV4/S-YL-69)
