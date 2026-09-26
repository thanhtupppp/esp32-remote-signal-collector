# Phase 1 — IR Capture Hardware

## Board

ESP32-C3 SuperMini.

## Recommended connection

Use a 3.3 V compatible demodulating IR receiver such as a VS1838B-class module.

```text
IR Receiver        ESP32-C3 SuperMini
-----------        ------------------
VCC       -------- 3V3
GND       -------- GND
OUT       -------- GPIO4
```

GPIO4 is the Phase 1 RAW capture input.

## Important

- Confirm the IR receiver module accepts 3.3 V before connecting it.
- Do not connect a 5 V output directly to an ESP32 GPIO.
- Keep the receiver away from strong sunlight and high-power LED lighting during initial tests.
- The firmware does not assume a specific remote protocol.
- Phase 1 captures the demodulated pulse train. It does not directly measure the original IR carrier frequency.

## Test procedure

1. Connect the receiver to 3V3, GND and GPIO4.
2. Flash `firmware/` with PlatformIO.
3. Open serial monitor at 115200 baud.
4. Point a normal IR remote at the receiver.
5. Press one button once.
6. Copy the JSON line printed by the ESP32.
7. Save the capture as a named signal only after verifying the waveform.

## Expected serial output

```text
ESP32-C3 SuperMini - IR Signal Collector
IR RX GPIO: 4
RMT capacity: 96 symbols (192 pulses max)
READY: press an IR remote button...
```

A successful capture is emitted as one JSON object per line.

## Phase 1 limitation

ESP32-C3 has two RMT memory blocks per RX channel in the Arduino RMT implementation. Long captures can therefore reach the buffer limit. The firmware marks these captures with `capture.truncated=true` so they are not silently treated as complete.
