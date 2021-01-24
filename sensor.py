import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import CONF_ID, CONF_LIGHT, UNIT_LUX, ICON_BRIGHTNESS_5

DEPENDENCIES = ['i2c']

ltr390_ns = cg.esphome_ns.namespace('ltr390')

LTR390Component = ltr390_ns.class_('LTR390Component', cg.PollingComponent, i2c.I2CDevice)

CONF_UV = 'uv'
UNIT_UVI = 'UVI'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(LTR390Component),

    cv.Optional(CONF_LIGHT): sensor.sensor_schema(UNIT_LUX, ICON_BRIGHTNESS_5, 1),
    cv.Optional(CONF_UV): sensor.sensor_schema(UNIT_UVI, ICON_BRIGHTNESS_5, 1),

}).extend(cv.polling_component_schema('60s')).extend(i2c.i2c_device_schema(0x53))

TYPES = {
    CONF_LIGHT: 'set_als_sensor',
    CONF_UV: 'set_uv_sensor',
}

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)

    for key, funcName in TYPES.items():

        if key in config:
            sens = yield sensor.new_sensor(config[key])
            cg.add(getattr(var, funcName)(sens))
