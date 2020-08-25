#include <ledMatrixDriver/displayContent/SdCharacterProvider.h>

#include <SPI.h>
#include <SD.h>

SdCharacterProvider::SdCharacterProvider()
{
    initSd();
}

SdCharacterProvider::SdCharacterProvider(uint8_t spiCsPin)
    : m_spiCsPin(spiCsPin)
{
    initSd();
}

//===============
// ICharacterProvider
//===============

bool getCharacter(String characterID, Character& characterStructToFill)
{
    
}

//===============
// Private
//===============

bool SdCharacterProvider::initSd()
{
    bool success = false;
    
    if(m_spiCsPin == 0xFF) {
        SD.begin(); // Defaults to hardware CS pin: https://www.arduino.cc/en/Reference/SDbegin
    } else {
        SD.begin(m_spiCsPin);
    }

    if(success == false) {
        // TODO: Log an error when the logger is created.
        while(1);
    }

    return success;
}