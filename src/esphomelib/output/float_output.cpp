#include "esphomelib/defines.h"

#ifdef USE_OUTPUT

#include "esphomelib/output/float_output.h"

#include "esphomelib/helpers.h"
#include "esphomelib/log.h"

ESPHOMELIB_NAMESPACE_BEGIN

namespace output {

void FloatOutput::set_max_power(float max_power) {
  this->max_power_ = clamp(0.0f, 1.0f, max_power);
}

float FloatOutput::get_max_power() const {
  return this->max_power_;
}

void FloatOutput::set_level(float state) {
  state = clamp(0.0f, 1.0f, state);

  if (state > 0.0f) { // ON
    // maybe refactor this
    if (this->power_supply_ != nullptr && !this->has_requested_high_power_) {
      this->power_supply_->request_high_power();
      this->has_requested_high_power_ = true;
    }
  } else { // OFF
    if (this->power_supply_ != nullptr && this->has_requested_high_power_) {
      this->power_supply_->unrequest_high_power();
      this->has_requested_high_power_ = false;
    }
  }

  float adjusted_value =  state * this->max_power_;
  if (this->is_inverted())
    adjusted_value = 1.0f - adjusted_value;
  this->write_state(adjusted_value);
}

void FloatOutput::write_state(bool state) {
  this->set_level(state != this->inverted_ ? 1.0f : 0.0f);
}

} // namespace output

ESPHOMELIB_NAMESPACE_END

#endif //USE_OUTPUT
