import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import CONF_ID, CONF_GAIN, CONF_RESOLUTION, UNIT_LUX, ICON_BRIGHTNESS_5

DEPENDENCIES = ['i2c']

ltr390_ns = cg.esphome_ns.namespace('ltr390')

LTR390Component = ltr390_ns.class_('LTR390Component', cg.PollingComponent, i2c.I2CDevice)

CONF_LIGHT = 'light'
CONF_ALS = 'als'
CONF_UVI = 'uvi'
CONF_UV = 'uv'
CONF_WFAC = 'wfac'

UNIT_COUNTS = '#'
UNIT_UVI = 'UVI'

ltr390_gain_t = ltr390_ns.enum('ltr390_gain_t')
GAIN_OPTIONS = {
    "X1": ltr390_gain_t.LTR390_GAIN_1,
    "X3": ltr390_gain_t.LTR390_GAIN_3,
    "X6": ltr390_gain_t.LTR390_GAIN_6,
    "X9": ltr390_gain_t.LTR390_GAIN_9,
    "X18": ltr390_gain_t.LTR390_GAIN_18,
}

ltr390_resolution_t = ltr390_ns.enum('ltr390_resolution_t')
RES_OPTIONS = {
    20: ltr390_resolution_t.LTR390_RESOLUTION_20BIT,
    19: ltr390_resolution_t.LTR390_RESOLUTION_19BIT,
    18: ltr390_resolution_t.LTR390_RESOLUTION_18BIT,
    17: ltr390_resolution_t.LTR390_RESOLUTION_17BIT,
    16: ltr390_resolution_t.LTR390_RESOLUTION_16BIT,
    13: ltr390_resolution_t.LTR390_RESOLUTION_13BIT,
}

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(LTR390Component),

    cv.Optional(CONF_LIGHT): sensor.sensor_schema(UNIT_LUX, ICON_BRIGHTNESS_5, 1),
    cv.Optional(CONF_ALS): sensor.sensor_schema(UNIT_COUNTS, ICON_BRIGHTNESS_5, 1),

    cv.Optional(CONF_UVI): sensor.sensor_schema(UNIT_UVI, ICON_BRIGHTNESS_5, 5),
    cv.Optional(CONF_UV): sensor.sensor_schema(UNIT_COUNTS, ICON_BRIGHTNESS_5, 1),

    cv.Optional(CONF_GAIN, default="X3"): cv.enum(GAIN_OPTIONS),
    cv.Optional(CONF_RESOLUTION, default="18"): cv.enum(RES_OPTIONS),
    cv.Optional(CONF_WFAC, default=1.0): cv.float_range(min=1.0),

}).extend(cv.polling_component_schema('60s')).extend(i2c.i2c_device_schema(0x53))

TYPES = {
    CONF_LIGHT: 'set_light_sensor',
    CONF_ALS: 'set_als_sensor',
    CONF_UVI: 'set_uvi_sensor',
    CONF_UV: 'set_uv_sensor',
}

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)

    cg.add(var.set_gain_value(config[CONF_GAIN]))
    cg.add(var.set_res_value(config[CONF_RESOLUTION]))
    cg.add(var.set_wfac_value(config[CONF_WFAC]))

    for key, funcName in TYPES.items():

        if key in config:
            sens = yield sensor.new_sensor(config[key])
            cg.add(getattr(var, funcName)(sens))
