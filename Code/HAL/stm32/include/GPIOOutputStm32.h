#include <stdint.h>

#include "IGPIOOutput.h"
#include "IGPIOOutputStm32.h"

class GPIOOutputStm32 : public IGPIOOutput, public IGPIOOutputStm32 {
   public:
    ~GPIOOutputStm32() override = default;

    //---------------
    // IGPIOOutput
    //---------------
    auto SetState(bool on) -> bool override;
    auto GetState() -> bool override;

    //---------------
    // IGPIOOutputStm32
    //---------------
    auto SetupConfiguration(const GPIOOutputConfigStm32&& gPIOOutputConfigStm32) -> bool override;
    auto GetConfiguration() -> const GPIOOutputConfigStm32& override;

   private:
    GPIOOutputConfigStm32 m_gPIOOutputConfigStm32;
};