#include <ledMatrixDriver/displayContent/CharacterProvider_AdafruitGFX.h>

#include <math.h> // ceil()

CharacterProvider_AdafruitGFX::CharacterProvider_AdafruitGFX()
{
    //setFont(FreeMono12pt7b);
}

//===============
// ICharacterProvider
//===============

bool CharacterProvider_AdafruitGFX::setFont(GFXfont& font)
{
    GFXglyph* fontGlyphs = font.glyph;
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

bool CharacterProvider_AdafruitGFX::getText(std::string text, ContentData& contentStructToFill)
{
    // !! TODO: No silent error
    // Check if all characters are availible in the active font.
    for(char& c : text) {
        if(c < m_activeFont->first)
        {
            return false;
        }
        if(c > m_activeFont->last)
        {
            return false;
        }
    }

    bool success = true;

    contentStructToFill.height = m_totalGlyphHeigt;
    uint8_t cursorX = 0;

    for(int i = 0; i < text.length(); i++) {
        uint16_t glyphIndex = text.at(i) - m_activeFont->first;
        GFXglyph glyph = m_activeFont->glyph[glyphIndex];

        // Dividing by 8 bits. The .0 is to make it a float which makes it possible to
        // use the ceil() function.
        const uint8_t numBytesFromTableNeeded = ceil((glyph.height * glyph.width) / 8.0);
        
        // (-1) to get the zero-based index
        uint8_t curRow = m_glyphBaseLineInTotalHeight + glyph.yOffset -1;
        contentStructToFill.clearData();

        // If the first character has a negative x offset
        // then the whole display should be shifted by that offset
        // in order to show the complete first character
        cursorX += glyph.xOffset;
        if(i == 0 && glyph.xOffset < 0) {
            cursorX = 0;
        }

        // !! TODO: Change the ContentData struct to probide the size of the mask to prevent
        // the magic numbers in this loop.

        // Fill the data struct row-by-row
        for(uint16_t i = glyph.bitmapOffset; i < numBytesFromTableNeeded && i < 32; i++) {
            uint8_t byteRow = m_activeFont->bitmap[i];
            contentStructToFill.contentMask[curRow] |= (byteRow << (31 - cursorX));
            curRow++;
        }
    }

    //contentStructToFill.width = curCol;
}