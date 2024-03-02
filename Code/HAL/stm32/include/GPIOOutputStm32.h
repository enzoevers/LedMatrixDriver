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
    auto SetOutputRegister(volatile uint32_t* outputRegister) -> bool override;
    auto GetOutputRegister() const -> volatile const uint32_t* override;

    auto SetPinMask(uint32_t pinMask) -> bool override;
    auto GetPinMask() const -> uint32_t override;

   private:
    volatile uint32_t* m_outputRegister;
    uint32_t m_pinMask;
};