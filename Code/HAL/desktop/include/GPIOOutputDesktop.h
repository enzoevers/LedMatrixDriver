#include <stdint.h>

#include "IGPIOOutput.h"
#include "IGPIOOutputDesktop.h"

class GPIOOutputDesktop : public IGPIOOutput, public IGPIOOutputDesktop {
   public:
    ~GPIOOutputDesktop() override = default;

    //---------------
    // IGPIOOutput
    //---------------
    auto SetState(bool on) -> bool override;
    auto GetState() -> bool override;

    //---------------
    // IGPIOOutputDesktop
    //---------------
    auto SetupConfiguration(const GPIOOutputConfigDesktop&& gPIOOutputConfigDesktop) -> void override;
    auto GetConfiguration() -> const GPIOOutputConfigDesktop& override;

   private:
    GPIOOutputConfigDesktop m_gPIOOutputConfigDesktop;
};