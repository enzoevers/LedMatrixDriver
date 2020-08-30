#pragma once 

#include <Arduino.h>
#include <stdint.h>
#include <ledMatrixDriver/displayContent/ICharacterProvider.h>


class SdCharacterProvider : public ICharacterProvider {
    public:
        SdCharacterProvider();
        SdCharacterProvider(uint8_t spiCsPin);

        //===============
        // ICharacterProvider
        //===============
        bool getCharacter(String characterID, Character& characterStructToFill) override;

    private:
        const uint8_t m_spiCsPin = 0xFF;

        bool initSd();
};