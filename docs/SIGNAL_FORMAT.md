# Signal Format v0.1

Schema JSON cho một lần capture. Phase 1 ưu tiên giữ nguyên RAW timing và logic level để dữ liệu có thể được phân tích lại.

```json
{
  "schema_version": "0.1",
  "id": "signal-0001",
  "device": {
    "name": "TV",
    "brand": "Example",
    "model": null
  },
  "button": "power",
  "signal": {
    "type": "IR",
    "carrier_hz": null,
    "protocol": null,
    "raw_us": [9000, 4500, 560, 560, 560, 1690],
    "raw_pulses": [
      { "level": 1, "duration_us": 9000 },
      { "level": 0, "duration_us": 4500 },
      { "level": 1, "duration_us": 560 },
      { "level": 0, "duration_us": 560 }
    ]
  },
  "capture": {
    "repeat_count": 1,
    "timestamp": null,
    "symbols": 3,
    "pulses": 6,
    "truncated": false,
    "gpio": 4,
    "tick_hz": 1000000
  }
}
```

## Quy ước

- `raw_us` là duration các pulse theo thứ tự capture, đơn vị microsecond.
- `raw_pulses` giữ cả `level` và `duration_us`, là dữ liệu nguồn ưu tiên cho phân tích nâng cao.
- `level` là logic level đọc trực tiếp từ chân IR receiver; module IR demodulator có thể cho tín hiệu active-low hoặc active-high tùy loại.
- `protocol` có thể `null` khi chưa nhận diện được.
- `carrier_hz` có thể `null` vì receiver demodulator thông thường không cung cấp trực tiếp carrier frequency.
- `truncated=true` nghĩa là capture chạm giới hạn bộ nhớ RMT và cần được xử lý riêng ở các phase sau.
- `tick_hz=1000000` nghĩa là 1 RMT tick tương ứng 1 microsecond.
- RF có thể bổ sung metadata modulation, bitrate, channel và frequency ở phiên bản schema sau.
- Không lưu secret hoặc credential trong dữ liệu capture.
