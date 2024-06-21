#include <stdint.h>

#include "IGPIOInput.h"
#include "IGPIOInputStm32.h"

namespace HAL::STM32 {

class GPIOInput : public HAL::IGPIOInput, public HAL::STM32::IGPIOInput {
   public:
    GPIOInput();
    ~GPIOInput() override = default;

    //---------------
    // HAL::GPIOInput
    //---------------
    auto GetState() -> bool override;

    //---------------
    // HAL::STM32::GPIOInput
    //---------------
    auto SetupConfiguration(const GPIOInputConfig&& gPIOInputConfig) -> bool override;
    auto GetConfiguration() -> const GPIOInputConfig& override;
    auto DebounceInterruptHandler() -> bool override;

   private:
    GPIOInputConfig m_gPIOInputConfig;

    bool m_stablePinState;
    bool m_previousPinState;
    uint8_t m_debounceCounter;
};

}  // namespace HAL::STM32