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


## Parameters

- `address`: The I2C address of the device. Default is `0x53`
- `update_interval`: How frequently to poll the device. Default is `60s`

## Complete configuration

```
  - platform: ltr390
    address: 0x12
    update_interval: 60s

```

## Datasheet

English datasheet https://optoelectronics.liteon.com/upload/download/DS86-2015-0004/LTR-390UV_Final_%20DS_V1%201.pdf

## Hardware

The hardware is available from Adafruit https://www.adafruit.com/product/4831
