#include <stdint.h>

#include "IGPIOOutput.h"
#include "IGPIOOutputStm32.h"

class GPIOOutputStm32 : public IGPIOOutput, public IGPIOOutputStm32 {
   public:
    ~GPIOOutputStm32() override = default;

    //---------------
    // IGPIOOutput
    //---------------
    auto SetState(bool on) -> void override;
    auto GetState() -> bool override;

    //---------------
    // IGPIOOutputStm32
    //---------------
    auto SetOutputRegister(uint32_t* outputRegister) -> void override;
    auto GetOutputRegister() const -> const uint32_t* override;

    auto SetPinMask(uint32_t pinMask) -> void override;
    auto GetPinMask() const -> uint32_t override;

   private:
    uint32_t* m_outputRegister;
    uint32_t m_pinMask;
};