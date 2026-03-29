/**
 * Sensor YL-69 (FC-28) — ESP32 — Humedad del Suelo
 * Pin Analógico: GPIO 34  (AO del módulo — ADC1, solo lectura)
 * Pin Digital  : GPIO 23  (DO del módulo — LOW cuando húmedo)
 *
 * Conexión física:
 *   VCC → 3.3 V o 5 V  (VIN del ESP32)
 *   GND → GND
 *   AO  → GPIO 34  (salida analógica)
 *   DO  → GPIO 23  (salida digital — umbral ajustable con potenciómetro)
 *
 * Lógica del sensor (LM393):
 *   ADC alto (~3200–3500) = suelo seco   (alta resistencia)
 *   ADC bajo  (~800–1500) = suelo húmedo (baja resistencia)
 *   DO = HIGH → suelo seco (por encima del umbral)
 *   DO = LOW  → suelo húmedo (por debajo del umbral)
 *
 * Calibración:
 *   VALOR_SECO  → leer el sensor con las sondas en el aire
 *   VALOR_HUMEDO → leer el sensor con las sondas sumergidas en agua
 *
 * Librería: ninguna (ADC nativo del ESP32, 12 bits)
 */

#define PIN_ANALOGICO  34
#define PIN_DIGITAL    23

/**
 * Ajustar estos valores según calibración propia.
 * Procedimiento: ver sección Calibración del README.
 */
#define VALOR_SECO    3200
#define VALOR_HUMEDO   800

const char* clasificarHumedad(int porcentaje) {
  if (porcentaje >= 80) return "MUY HUMEDO";
  if (porcentaje >= 60) return "HUMEDO";
  if (porcentaje >= 40) return "OPTIMO";
  if (porcentaje >= 20) return "SECO";
  return "MUY SECO";
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_DIGITAL, INPUT);
  Serial.println(F("=== YL-69 en ESP32 — Humedad del Suelo ==="));
  Serial.println(F("----------------------------------"));
}

void loop() {
  int rawADC       = analogRead(PIN_ANALOGICO);
  int estadoDigital = digitalRead(PIN_DIGITAL);

  /** Mapeo invertido: ADC alto = seco (0%), ADC bajo = húmedo (100%) */
  int porcentaje = map(rawADC, VALOR_SECO, VALOR_HUMEDO, 0, 100);
  porcentaje = constrain(porcentaje, 0, 100);

  Serial.print(F("ADC (raw)     : "));
  Serial.println(rawADC);

  Serial.print(F("Humedad       : "));
  Serial.print(porcentaje);
  Serial.println(F(" %"));

  Serial.print(F("Estado        : "));
  Serial.println(clasificarHumedad(porcentaje));

  Serial.print(F("Digital DO    : "));
  Serial.println(estadoDigital == LOW ? F("HUMEDO (umbral superado)") : F("SECO"));

  Serial.println(F("----------------------------------"));
  delay(1000);
}
