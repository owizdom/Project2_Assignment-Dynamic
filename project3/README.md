# Project 3: Dynamic IoT Event Monitoring and Callback Engine

## 1. Source code

```
gcc *.c -o iot_monitor
./iot_monitor
```

- [`iot_monitor.c`](iot_monitor.c): the main program. It holds the 6 starting devices and `main`, which seeds `rand()`, runs the menu through an array of function pointers and frees the memory at the end.
- [`iot_monitor.h`](iot_monitor.h): the structs, including the reading union, and every other function. That covers register and remove, where the array grows and shrinks with `realloc`, and the random readings. It also has the three monitors, the drift detector, `process_device`, the alert list and the summary.

## 2. Device types and limits

| Type | Union member | Simulated range | Normal range | Callback |
|---|---|---|---|---|
| Temperature | `float temperature` | 10.0 to 50.0 C | 15.0 to 45.0 C | `temperature_monitor` |
| Pressure | `int pressure` | 80 to 120 kPa | 90 to 110 kPa | `pressure_monitor` |
| Battery | `float voltage` | 3.00 to 4.40 V | 3.30 to 4.20 V | `battery_monitor` |

Readings come from `rand()`, seeded with `srand(time(NULL))`. All limits are `#define`s in `iot_monitor.h`. Each reading goes through `process_device` twice. The first call uses its type's monitor, picked from an array of function pointers. The second uses the drift detector.

## 3. Sample output

[`sample_run/output.txt`](sample_run/output.txt) is a real run, typed from [`sample_run/input.txt`](sample_run/input.txt). It shows 20 readings from 7 devices and all three monitors on normal and abnormal readings. It also shows registering and removing devices, the alert list and the summary.

