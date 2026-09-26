# Firmware

Khu vực firmware cho ESP32-S3.

Phase 1 dự kiến dùng PlatformIO hoặc ESP-IDF/Arduino làm nền, với lớp HAL tách riêng khỏi capture/parser để có thể thay đổi phần cứng.

## Dự kiến module

- `capture/` — GPIO/RMT capture
- `protocol/` — decoder và detector
- `storage/` — serialization
- `api/` — Wi-Fi/API
- `app/` — state machine của thiết bị