#include <stdint.h>

#include "IGPIOOutput.h"

class GPIOOutputDesktop : public IGPIOOutput {
   public:
    GPIOOutputDesktop() = default;
    ~GPIOOutputDesktop() override = default;

    //---------------
    // IGPIOOutput
    //---------------
    auto SetState(bool on) -> void override;
    auto GetState() -> bool override;
};