#include <LedMatrixDriver/displayContent/CharacterProvider_AdafruitGFX.h>

#include <math.h> // ceil()

//===============
// ICharacterProvider
//===============

bool CharacterProvider_AdafruitGFX::setFont(GFXfont& font)
{
    GFXglyph* fontGlyphs = font->glyph;
    unsigned int glyphCount = m_activeFont->last - m_activeFont->first + 1;
    uint8_t maxPointsAboveBaseline = 0;
    uint8_t maxPointsBelowBaseline = 0;

    for(unsigned int i = 0; i < glyphCount; i++) {
        int8_t yOffset = fontGlyphs[i].yOffset;

        if(yOffset > 0) {
            if(maxPointsBelowBaseline < yOffset) {
                maxPointsBelowBaseline = yOffset;
            }
        } else if(yOffset < 0) {
            yOffset *= -1; // Make it possitive

            if(maxPointsAboveBaseline < yOffset) {
                maxPointsAboveBaseline = yOffset;
            }
        }
        
        uint8_t pointsBelowGlypsBaseline = yOffset + fontGlyphs[i].height;

        if(maxPointsBelowBaseline < pointsBelowGlypsBaseline) {
            maxPointsBelowBaseline = pointsBelowGlypsBaseline;
        }
    }
    
    m_activeFont = &font;
    m_totalGlyphHeigt = maxPointsAboveBaseline + maxPointsBelowBaseline;
    m_glyphBaseLineInTotalHeight = maxPointsAboveBaseline;
}

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