# Signal Format v0.1

Schema JSON tối thiểu cho một lần capture.

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
    "carrier_hz": 38000,
    "protocol": null,
    "raw_us": [9000, 4500, 560, 560, 560, 1690]
  },
  "capture": {
    "repeat_count": 1,
    "timestamp": null
  }
}
```

## Quy ước

- `raw_us` là mảng duration HIGH/LOW xen kẽ theo thứ tự capture.
- Đơn vị thời gian là microsecond.
- `protocol` có thể `null` khi chưa nhận diện được.
- `carrier_hz` có thể `null` khi không xác định.
- RF có thể bổ sung metadata modulation, bitrate hoặc channel ở phiên bản schema sau.
- Không lưu secret hoặc credential trong dữ liệu capture.