This is an [esphome](https://esphome.io) component for the LTR390 (ltr390) UV and ambient light sensor.

[<img src="https://cdn-shop.adafruit.com/970x728/4831-04.jpg" width="400px">](https://www.adafruit.com/product/4831)

## Installation
Copy the `ltr390` directory into your ESPHome `custom_components` directory (creating it if it does not exist).

## Dependencies
The [I2C Bus](https://esphome.io/components/i2c.html#i2c) must be set up in order for this component to work.

## Minimal configuration
The following configuration shows the minimal set up to read temperature and humidity from the sensor.
```yaml
i2c:

sensor:
  - platform: ltr390
    uv:
      name: "LTR390 UV"
    light:
      name: "LTR390 Light"
```

## Available Sensors

- `light`: calculated lux (lx)
- `als`: raw light sensor count (#)
- `uvi`: calculated UV Index (UVI)
- `uv`: raw UV sensor count (#)

## Optional Parameters

- `gain`: Adjusts the sensitivity of the sensor. A larger value means higher sensitivity. See table below for details. Default is ``"X3"``.
- `resolution`: ADC resolution. Higher resolutions require longer sensor integration times. See table below for details. Default is `18`.
- `wfac`: Window correction factor. Use larger values when using under tinted windows. Default is 1.0, must be >= 1.0.
- `address`: The I2C address of the device. Default is `0x53`.
- `update_interval`: How frequently to poll the device. It is recommended that the update interval is at least 1 second since updates can take up to 800ms when using a high resolution value. Default is `60s`.

## Complete configuration

```
- platform: ltr390
  light:
    name: "LTR390 Light"
  als:
    name: "LTR390 ALS Count"
  uvi:
    name: "LTR390 UVI"
  uv:
    name: "LTR390 UV"
  gain: "X3"
  resolution: 18
  wfac: 1
  address: 0x53
  update_interval: 60s

```

## Lux and UVI Formulas

<img src="https://latex.codecogs.com/gif.latex?\text{lux}&space;=&space;\frac{0.6&space;\times&space;\text{als}}{\text{gain}&space;\times&space;\text{int}}&space;\times&space;\text{wfac}" />

<img src="https://latex.codecogs.com/gif.latex?\text{UVI}&space;=&space;\frac{\text{uv}}{\text{sensitivity}}&space;\times&space;\text{wfac}" />

where:
- `als` and `uv` are the sensor values
- `gain` is the amount of gain, see the table below for details
- `int` is the integration time in 100s of ms and is tied to the resolution, see the table below for details
- `sensitivity` has the value `2300` and is the sensor's count per UVI
- `wfac` is the window correction factor

<table>
<tr><td>

| Gain Parameter | gain       |
|----------------|------------|
| X1             | 1          |
| X3             | 3          |
| X6             | 6          |
| X9             | 9          |
| X18            | 18         |

</td><td>

| Resolution (bits) | Integration Time (ms) | int  |
|-------------------|-----------------------|------|
| 16                | 25                    | 0.25 |
| 17                | 50                    | 0.5  |
| 18                | 100                   | 1    |
| 19                | 200                   | 2    |
| 20                | 400                   | 4    |

</td></tr>
</table>

## Datasheet

[English datasheet](https://optoelectronics.liteon.com/upload/download/DS86-2015-0004/LTR-390UV_Final_%20DS_V1%201.pdf)

## Hardware

The hardware is available from Adafruit https://www.adafruit.com/product/4831
