#include "ltr390.h"
#include "esphome/core/log.h"


namespace esphome {
namespace ltr390 {

static const char *TAG = "ltr390";

bool LTR390Component::enabled(void) {
  std::bitset<8> crtl_value (this->ctrl_reg_->get());
  return (bool)crtl_value[LTR390_CTRL_EN];
}

void LTR390Component::enable(bool en) {
  std::bitset<8> crtl_value (this->ctrl_reg_->get());
  crtl_value[LTR390_CTRL_EN] = en;
  *this->ctrl_reg_ = crtl_value.to_ulong();
}

bool LTR390Component::reset(void) {
  std::bitset<8> crtl_value (this->ctrl_reg_->get());

  crtl_value[LTR390_CTRL_RST] = 1;
  *this->ctrl_reg_ = crtl_value.to_ulong();

  delay(10);
  // Read after reset
  crtl_value = std::bitset<8>(this->ctrl_reg_->get());
  if (crtl_value.to_ulong()) {
    return false;
  }

  return true;
}

void LTR390Component::set_mode(ltr390_mode_t mode) {
  std::bitset<8> crtl_value (this->ctrl_reg_->get());
  crtl_value[LTR390_CTRL_MODE] = mode;
  *this->ctrl_reg_ = crtl_value.to_ulong();
}

ltr390_mode_t LTR390Component::get_mode(void) {
  std::bitset<8> crtl_value (this->ctrl_reg_->get());
  return (ltr390_mode_t)(int)crtl_value[LTR390_CTRL_MODE];
}

void LTR390Component::set_gain(ltr390_gain_t gain) {
  *this->gain_reg_ = gain;
}

ltr390_gain_t LTR390Component::get_gain(void) {
  std::bitset<8> gain_value (this->gain_reg_->get());
  return (ltr390_gain_t)gain_value.to_ulong();
}

void LTR390Component::set_resolution(ltr390_resolution_t res) {
  std::bitset<8> res_value (this->res_reg_->get());

  std::bitset<3> new_res_value (res);

  for (int i = 0; i < 3; i++) {
    res_value[4+i] = new_res_value[i];
  }

  *this->res_reg_ = res_value.to_ulong();
}

ltr390_resolution_t LTR390Component::get_resolution(void) {
  std::bitset<8> res_value (this->res_reg_->get());

  std::bitset<3> output_value (0);
  for (int i = 0; i < 3; i++) {
    output_value[i] = res_value[4+i];
  }

  return (ltr390_resolution_t)output_value.to_ulong();
}

bool LTR390Component::new_data_available(void) {
  std::bitset<8> status_value (this->status_reg_->get());
  return (bool)status_value[3];
}

uint32_t bytes_to_int(uint8_t *buffer, uint8_t num_bytes) {
  uint32_t value = 0;

  for (int i = 0; i < num_bytes; i++) {
    value <<= 8;
    value |= buffer[num_bytes - i - 1];
  }

  return value;
}

uint32_t LTR390Component::read_sensor_data(ltr390_mode_t mode) {

  this->set_mode(mode);

  delay(this->resolution_values_[LTR390_RESOLUTION_18BIT] * 100);
  while (!this->new_data_available()) {
    delay(this->resolution_values_[LTR390_RESOLUTION_18BIT] * 100);
  }

  const uint8_t num_bytes = 3;
  uint8_t buffer[num_bytes];

  this->read_bytes(this->mode_addresses_[mode], buffer, num_bytes);

  return bytes_to_int(buffer, num_bytes);
}

void LTR390Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ltr390...");

  this->ctrl_reg_ = new i2c::I2CRegister(this, LTR390_MAIN_CTRL);
  this->status_reg_ = new i2c::I2CRegister(this, LTR390_MAIN_STATUS);
  this->gain_reg_ = new i2c::I2CRegister(this, LTR390_GAIN);
  this->res_reg_ = new i2c::I2CRegister(this, LTR390_MEAS_RATE);

  this->reset();

  this->enable(true);
  ESP_LOGD(TAG, "%s", this->enabled() ? "ENABLED" : "DISABLED");
  if (!this->enabled()) {
    this->mark_failed();
    return;
  }
}

void LTR390Component::dump_config() {
    LOG_I2C_DEVICE(this);
}

void LTR390Component::update() {


      uint32_t wfac = 1;

      if (this->light_sensor_ != nullptr ||  this->als_sensor_ != nullptr) {

        uint32_t als = this->read_sensor_data(LTR390_MODE_ALS);

        if (this->light_sensor_ != nullptr) {

          float lux = (0.6 * als) / (this->gain_values_[LTR390_GAIN_3] * this->resolution_values_[LTR390_RESOLUTION_18BIT]) * wfac;
          this->light_sensor_->publish_state(lux);
        }

        if (this->als_sensor_ != nullptr) {
          this->als_sensor_->publish_state(als);
        }
      }

      if (this->uvi_sensor_ != nullptr || this->uv_sensor_ != nullptr) {

        uint32_t uv = this->read_sensor_data(LTR390_MODE_UVS);

        if (this->uvi_sensor_ != nullptr) {
          this->uvi_sensor_->publish_state(uv/LTR390_SENSITIVITY * wfac);
        }

        if (this->uv_sensor_ != nullptr) {
          this->uv_sensor_->publish_state(uv);
        }

      }
}

}  // namespace ltr390
}  // namespace esphome
