#include <LedMatrixDriver/displayContent/CharacterProvider_AdafruitGFX.h>

#include <math.h> // ceil()

//===============
// ICharacterProvider
//===============

bool CharacterProvider_AdafruitGFX::getCharacter(uint16_t characterID, ContentData& contentStructToFill)
{
    // !! TODO: No silent error
    if(characterID < m_activeFont->first)
    {
        return false;
    }
    if(characterID > m_activeFont->last)
    {
        return false;
    }

    bool success = true;

    uint16_t glyphIndex = characterID - m_activeFont->first;
    GFXglyph* glyph = m_activeFont->glyph[glyphIndex];

    contentStructToFill.height = glyph->height;
    contentStructToFill.width = glyph->xAdvance;

    const numBytesFromTableNeeded = ceil((glyph->height * glyph->width) / 8.0);
    
    bool newRow = true;
    uint8_t curRow = 0;
    uint8_t curCol = 0;
    contentStructToFill.clearData();
    for(uint16_t i = glyph->bitmapOffset; i < numBytesFromTableNeeded; i++) {
        if(newRow) {
            newRow = false;
        }
        contentStructToFill.contentMask
    }
}