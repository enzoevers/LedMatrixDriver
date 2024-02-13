#include <stdint.h>

#include "IGPIOOutput.h"

class GPIOOutputStm32 : public IGPIOOutput {
   public:
    GPIOOutputStm32(uint32_t* outputRegister, uint32_t pinMask);
    ~GPIOOutputStm32() override = default;

    //---------------
    // IGPIOOutput
    //---------------
    auto SetState(bool on) -> void override;
    auto GetState() -> bool override;

   private:
    uint32_t* m_outputRegister;
    uint32_t m_pinMask;
};