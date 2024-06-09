#include <stdint.h>

#include "IGPIOOutput.h"
#include "IGPIOOutputStm32.h"

namespace HAL::STM32 {

class GPIOOutput : public HAL::IGPIOOutput, public HAL::STM32::IGPIOOutput {
   public:
    ~GPIOOutput() override = default;

    //---------------
    // HAL::IGPIOOutput
    //---------------
    auto SetState(bool on) -> bool override;
    auto GetState() -> bool override;

    //---------------
    // HAL::STM32::IGPIOOutput
    //---------------
    auto SetupConfiguration(const GPIOOutputConfig&& gPIOOutputConfig) -> bool override;
    auto GetConfiguration() -> const GPIOOutputConfig& override;

   private:
    GPIOOutputConfig m_gPIOOutputConfig;
};

}  // namespace HAL::STM32