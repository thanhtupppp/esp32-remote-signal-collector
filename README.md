# ESP32 Remote Signal Collector

Nền tảng thu thập, phân tích và lưu trữ tín hiệu điều khiển từ remote để phục vụ tự động hóa và điều khiển thiết bị ở các phiên bản sau.

## Mục tiêu

- Thu RAW waveform từ remote IR và RF.
- Giữ nguyên dữ liệu gốc để có thể phân tích lại.
- Nhận diện protocol, carrier/frequency và timing khi có thể.
- Gắn metadata thiết bị, model và nút bấm.
- Chuẩn hóa dữ liệu để sau này phát lại bằng ESP32.
- Có nền tảng Web/API để quản lý thư viện tín hiệu.

## Kiến trúc dự kiến

```text
Remote IR/RF
    │
    ▼
Capture frontend
    │
    ▼
ESP32-S3
    │
    ├── RAW capture
    ├── Protocol detection
    ├── Signal metadata
    └── Local storage
    │
    ▼
Wi-Fi API / Web UI
    │
    ▼
Signal database
```

## Phần cứng dự kiến

- ESP32-S3
- IR receiver demodulator
- IR LED transmitter
- CC1101 cho RF sub-GHz phù hợp
- Bộ nhớ flash/SD khi cần lưu capture lớn

## Nguyên tắc dữ liệu

RAW timing là nguồn dữ liệu quan trọng nhất. Việc nhận diện protocol chỉ là lớp metadata bổ sung; capture chưa nhận diện được vẫn phải được lưu.

## Cấu trúc repository

```text
firmware/        Firmware ESP32
docs/            Đặc tả kiến trúc và định dạng
data/            Dữ liệu mẫu
tools/            Công cụ phân tích sau này
tests/            Test parser/decoder sau này
```

## Roadmap

### Phase 0 — Foundation

- Repository skeleton
- Signal schema v0.1
- Hardware abstraction plan

### Phase 1 — IR Capture

- RAW IR capture
- Serial output
- JSON export

### Phase 2 — Signal Analysis

- Protocol detection
- Timing normalization
- Capture validation

### Phase 3 — RF Capture

- CC1101 integration
- RF capture metadata
- RAW packet storage

### Phase 4 — Signal Library

- Web UI
- Device/button catalog
- Search and tagging

### Phase 5 — Playback

- IR playback
- RF playback where technically appropriate
- Device control API

## Lưu ý

Một số hệ thống RF dùng rolling code, mã hóa hoặc cơ chế xác thực. Với các hệ thống đó, việc capture RAW không đồng nghĩa với khả năng tạo remote thay thế.

## License

License sẽ được xác định khi phạm vi dự án ổn định.