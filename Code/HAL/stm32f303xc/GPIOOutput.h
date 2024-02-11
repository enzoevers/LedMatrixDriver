#include "IGPIOOutput.h"

class GPIOOutput : public IGPIOOutput {
    GPIOOutput();
    ~GPIOOutput() override = default;

    auto SetState(bool on) -> void override;
    auto GetState() -> bool override;
};