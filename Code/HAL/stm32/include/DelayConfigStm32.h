#pragma once

#include <stdint.h>

struct DelayConfigstm32 {
    volatile uint32_t *pStatusRegister;
    /*! \note It is assumes that the reload register can have a value of max 16 bits */
    volatile uint32_t *pAutoReloadRegister;
    volatile uint32_t *pControlRegister;
    /*! \note It is assumes that the prescaler can have a value of max 16 bits */
    volatile uint32_t *pPrescalerRegister;
    uint32_t timerInputFrequencyInHertz;
    uint32_t updateInterruptMask;
    uint32_t counterEnableMask;

    auto operator==(const DelayConfigstm32 &rhs) const -> bool {
        return (this->pStatusRegister == rhs.pStatusRegister && this->pAutoReloadRegister == rhs.pAutoReloadRegister &&
                this->pControlRegister == rhs.pControlRegister && this->pPrescalerRegister == rhs.pPrescalerRegister &&
                this->timerInputFrequencyInHertz == rhs.timerInputFrequencyInHertz &&
                this->updateInterruptMask == rhs.updateInterruptMask &&
                this->counterEnableMask == rhs.counterEnableMask);
    }
};