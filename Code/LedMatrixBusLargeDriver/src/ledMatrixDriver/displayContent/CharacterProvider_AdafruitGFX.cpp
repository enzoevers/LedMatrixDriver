#include <ledMatrixDriver/displayContent/CharacterProvider_AdafruitGFX.h>
#include <vector>

// Fonts
#include <Adafruit_GFX/Fonts/FreeMono12pt7b.h>
//#include <Adafruit_GFX/Fonts/....h>
//#include <Adafruit_GFX/Fonts/....h>
//etc

#include <math.h> // ceil()

#include <iostream>
#include <bitset>

CharacterProvider_AdafruitGFX::CharacterProvider_AdafruitGFX()
{
    setFont(FreeMono12pt7b);
}

//===============
// ICharacterProvider
//===============

bool CharacterProvider_AdafruitGFX::setFont(GFXfont& font)
{
    bool success = true;

    const GFXglyph* fontGlyphs = font.glyph;
    unsigned int glyphCount = font.last - font.first + 1;
    uint8_t maxPointsAboveBaseline = 0;
    uint8_t maxPointsBelowBaseline = 0;

    for(unsigned int i = 0; i < glyphCount; i++) {
        int8_t yOffset = fontGlyphs[i].yOffset;
        int8_t height = fontGlyphs[i].height;

        if(yOffset > 0) {
            if(maxPointsBelowBaseline < yOffset) {
                maxPointsBelowBaseline = yOffset;
            }
        } else if(yOffset < 0) {
            yOffset *= -1; // Make it possitive

            if(maxPointsAboveBaseline < yOffset) {
                maxPointsAboveBaseline = yOffset;
            }

            yOffset *= -1; // Make it negative again to be used later
        }
        
        int8_t pointsBelowGlypsBaseline = yOffset + height;

        if((pointsBelowGlypsBaseline > 0) && maxPointsBelowBaseline < pointsBelowGlypsBaseline) {
            maxPointsBelowBaseline = pointsBelowGlypsBaseline;
        }
    }

    // !! TODO: Detect if a certain in-betweeen character code is missing

    m_activeFont = &font;
    m_totalGlyphHeigt = maxPointsAboveBaseline + maxPointsBelowBaseline;
    m_glyphBaseFromTop = maxPointsAboveBaseline;

    return success;
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

    contentStructToFill.clearData();

    contentStructToFill.height = m_totalGlyphHeigt;
    uint8_t cursorX = 0;
    bool needExtraContectStructForText = false;

    for(int c = 0; c < text.length(); c++) {
        uint16_t glyphIndex = text.at(c) - m_activeFont->first;
        GFXglyph glyph = m_activeFont->glyph[glyphIndex];

        const uint16_t numPixelsNedded = glyph.height * glyph.width;

        // Dividing by 8 bits. The .0 is to make it a float which makes it possible to
        // use the ceil() function.
        const uint8_t numBytesFromTableNeeded = ceil((numPixelsNedded) / 8.0);

        std::vector<uint8_t> pixelArray = {};
        
        // The glyph.yOffset if negative for most 'normal' characters
        int8_t curY = m_glyphBaseFromTop + glyph.yOffset;

        // The lazy option

        // Fill the data struct row-by-row
        for(uint16_t i = glyph.bitmapOffset; i < (glyph.bitmapOffset + numBytesFromTableNeeded); i++) {
            uint8_t charByte = m_activeFont->bitmap[i];
            
            // It can be that in the last byte only a few bits belong to the current character
            int8_t lastBit = 0;
            if(i == (glyph.bitmapOffset + numBytesFromTableNeeded) -1) {
                lastBit = ((numBytesFromTableNeeded*8)-numPixelsNedded);
            }

            for(int8_t b = 7; b >=lastBit ; b--) {
                uint8_t activeBit = ((charByte & (0x1 << b)) >> b);
                pixelArray.push_back(activeBit);
            }
        }

        for(uint16_t y = 0; y < glyph.height; y++) {
            for(uint8_t x = 0; x < glyph.width; x++) {
                uint16_t index = y*glyph.width + x;
                if(pixelArray.at(index)) {
                    contentStructToFill.contentMask[curY] |= ((uint64_t)0x1 << ((contentStructToFill.maxRowWidth-1) - cursorX - x));
                }                
            }   
            curY++;
            if(curY == contentStructToFill.maxRows) {
                curY = contentStructToFill.maxRows;
            }
        }

        cursorX += glyph.xAdvance + glyph.xOffset;

        if(cursorX > contentStructToFill.maxRowWidth) {
            contentStructToFill.width = contentStructToFill.maxRowWidth;
        } else {
            contentStructToFill.width = cursorX;
        }
    }

    return success;
}