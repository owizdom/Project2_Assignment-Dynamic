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

## 4. Custom callback: Sensor Drift Detector

It flags a reading that jumps too far from the device's previous reading. The drift limit is 15.0 C, 15 kPa or 0.50 V, by type. This catches a failing sensor even when both readings are normal. A device's first reading never triggers it.

It reads the value from the union member that matches the device's type and takes the size of the change from the previous reading. If the change is over the limit, it raises a "sudden jump" alert. The new value then becomes the previous one.

In the sample run it found 4 jumps, on devices 301, 302 (twice) and 401. Device 401 went from 4.20 V to 3.69 V, both inside the normal range, so only the drift detector caught it.

## 5. Registration and removal

From the sample run, which starts with 6 devices:

```
Choice: 1
Device id: 101
Rejected: id 101 already exists.

Choice: 1
Device id: 401
Name: Wind Turbine
Type 1) Temperature  2) Pressure  3) Battery: 3
Registered Wind Turbine. Devices: 7

Choice: 3
Id to remove: 202
Removed device 202. Devices: 6

Choice: 3
Id to remove: 999
No device with that id.
```

## 6. Normal readings and alerts

The second round of readings, one line per reading with the callback that processed it:

```
101  Server Room    Temperature  45.3 C    ALERT too hot (temperature_monitor)
102  Boiler Room    Temperature  39.1 C    Normal (temperature_monitor)
201  Water Pump     Pressure     119 kPa   ALERT too high (pressure_monitor)
202  Air Tank       Pressure     102 kPa   Normal (pressure_monitor)
301  Gate Sensor    Battery      3.06 V    ALERT too low (battery_monitor) sudden jump (drift_detector)
302  Door Lock      Battery      3.25 V    ALERT too low (battery_monitor) sudden jump (drift_detector)
401  Wind Turbine   Battery      4.20 V    Normal (battery_monitor)
```

Each alert in the alert list shows the device, the value and unit, the expected range, the reason and the callback:

```
101  Server Room    45.3 C, expected 15.0 to 45.0 C: too hot (temperature_monitor)
301  Gate Sensor    3.06 V, expected change up to 0.50 V: sudden jump (drift_detector)
```

## Author

Wisdom Okechukwu
