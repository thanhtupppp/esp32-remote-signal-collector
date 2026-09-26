#include <Arduino.h>
#include <ArduinoJson.h>

// ESP32-C3 SuperMini / Phase 1 IR collector.
// Typical demodulating IR receiver output is connected to GPIO4.
constexpr int IR_RX_GPIO = 4;
constexpr uint32_t RMT_TICK_HZ = 1'000'000; // 1 tick = 1 us
constexpr uint8_t RMT_BLOCKS = RMT_MEM_NUM_BLOCKS_2;
constexpr size_t MAX_SYMBOLS = RMT_BLOCKS * RMT_SYMBOLS_PER_CHANNEL_BLOCK;
constexpr uint16_t RX_MIN_PULSE_US = 80;
constexpr uint16_t RX_IDLE_US = 15'000;
constexpr uint16_t READ_TIMEOUT_MS = 250;

static rmt_data_t rmtBuffer[MAX_SYMBOLS];

void printCapture(const rmt_data_t* data, size_t symbols) {
  // The project schema keeps raw_us as alternating pulse durations.
  // raw_pulses additionally preserves the measured logic level.
  StaticJsonDocument<12288> doc;
  doc["schema_version"] = "0.1";
  doc["id"] = "capture-runtime";
  doc["device"]["name"] = "unknown";
  doc["device"]["brand"] = nullptr;
  doc["device"]["model"] = nullptr;
  doc["button"] = "unknown";

  JsonObject signal = doc["signal"].to<JsonObject>();
  signal["type"] = "IR";
  signal["carrier_hz"] = nullptr;
  signal["protocol"] = nullptr;
  JsonArray rawUs = signal["raw_us"].to<JsonArray>();
  JsonArray rawPulses = signal["raw_pulses"].to<JsonArray>();

  size_t pulseCount = 0;
  bool truncated = symbols >= MAX_SYMBOLS;

  for (size_t i = 0; i < symbols && pulseCount < MAX_SYMBOLS * 2; ++i) {
    const rmt_data_t& s = data[i];

    if (s.duration0 > 0) {
      JsonObject pulse = rawPulses.add<JsonObject>();
      pulse["level"] = static_cast<uint8_t>(s.level0);
      pulse["duration_us"] = s.duration0;
      rawUs.add(s.duration0);
      ++pulseCount;
    }

    if (s.duration1 > 0) {
      JsonObject pulse = rawPulses.add<JsonObject>();
      pulse["level"] = static_cast<uint8_t>(s.level1);
      pulse["duration_us"] = s.duration1;
      rawUs.add(s.duration1);
      ++pulseCount;
    }
  }

  JsonObject capture = doc["capture"].to<JsonObject>();
  capture["repeat_count"] = 1;
  capture["timestamp"] = millis();
  capture["symbols"] = symbols;
  capture["pulses"] = pulseCount;
  capture["truncated"] = truncated;
  capture["gpio"] = IR_RX_GPIO;
  capture["tick_hz"] = RMT_TICK_HZ;

  serializeJson(doc, Serial);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(300);

  Serial.println();
  Serial.println(F("ESP32-C3 SuperMini - IR Signal Collector"));
  Serial.printf("IR RX GPIO: %d\n", IR_RX_GPIO);
  Serial.printf("RMT capacity: %u symbols (%u pulses max)\n",
                static_cast<unsigned>(MAX_SYMBOLS),
                static_cast<unsigned>(MAX_SYMBOLS * 2));

  if (!rmtInit(IR_RX_GPIO, RMT_RX_MODE, RMT_BLOCKS, RMT_TICK_HZ)) {
    Serial.println(F("ERROR: rmtInit() failed"));
    while (true) {
      delay(1000);
    }
  }

  if (!rmtSetRxMinThreshold(IR_RX_GPIO, RX_MIN_PULSE_US)) {
    Serial.println(F("WARNING: rmtSetRxMinThreshold() failed"));
  }

  if (!rmtSetRxMaxThreshold(IR_RX_GPIO, RX_IDLE_US)) {
    Serial.println(F("WARNING: rmtSetRxMaxThreshold() failed"));
  }

  Serial.println(F("READY: press an IR remote button..."));
}

void loop() {
  size_t symbols = MAX_SYMBOLS;

  // Blocking read keeps Phase 1 simple and deterministic.
  const bool readOk = rmtRead(IR_RX_GPIO, rmtBuffer, &symbols, READ_TIMEOUT_MS);
  if (!readOk || !rmtReceiveCompleted(IR_RX_GPIO) || symbols == 0) {
    return;
  }

  printCapture(rmtBuffer, symbols);
}
