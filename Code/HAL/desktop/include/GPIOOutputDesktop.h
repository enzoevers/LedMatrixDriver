#include <stdint.h>

#include "IGPIOOutput.h"
#include "IGPIOOutputDesktop.h"

class GPIOOutputDesktop : public IGPIOOutput, public IGPIOOutputDesktop {
   public:
    ~GPIOOutputDesktop() override = default;

    //---------------
    // IGPIOOutput
    //---------------
    auto SetState(bool on) -> void override;
    auto GetState() -> bool override;

    //---------------
    // IGPIOOutputDesktop
    //---------------
    auto SetOutputRegister(uint32_t* outputRegister) -> void override;
    auto GetOutputRegister() const -> uint32_t* override;

    auto SetPinMask(uint32_t pinMask) -> void override;
    auto GetPinMask() const -> uint32_t override;

   private:
    uint32_t* m_outputRegister;
    uint32_t m_pinMask;
};