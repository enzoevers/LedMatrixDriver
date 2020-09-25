#include <ledMatrixDriver/displayContent/CharacterProvider_AdafruitGFX.h>

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
    std::cout << "Setting the font\n";
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
        
        uint8_t pointsBelowGlypsBaseline = fontGlyphs[i].yOffset + fontGlyphs[i].height;

        if(maxPointsBelowBaseline < pointsBelowGlypsBaseline) {
            maxPointsBelowBaseline = pointsBelowGlypsBaseline;
        }
    }

    // !! TODO: When detect is a certain in-betweeen character code is missing

    m_activeFont = &font;
    m_totalGlyphHeigt = maxPointsAboveBaseline + maxPointsBelowBaseline;
    m_glyphBaseFromTop = maxPointsAboveBaseline;

    std::cout << "m_totalGlyphHeigt: " << int(m_totalGlyphHeigt) << "\n";
    std::cout << "m_glyphBaseFromTop: " << int(m_glyphBaseFromTop) << "\n";

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

    std::cout << "Printing text '" << text << "' to the screen\n";
    for(int i = 0; i < text.length(); i++) {
        std::cout << "Printing character '" << text.at(i) << "' to the screen\n";
        std::cout << "Bitmap index: " << int(text.at(i) - m_activeFont->first) << "\n";
        uint16_t glyphIndex = text.at(i) - m_activeFont->first;
        GFXglyph glyph = m_activeFont->glyph[glyphIndex];

        // Dividing by 8 bits. The .0 is to make it a float which makes it possible to
        // use the ceil() function.
        const uint8_t numBytesFromTableNeeded = ceil((glyph.height * glyph.width) / 8.0);
        std::cout << "numBytesFromTableNeeded: " << int(numBytesFromTableNeeded) << "\n";
        
        uint8_t overlapBitsRemaining = 0;
        // (-1) to get the zero-based index
        uint8_t curY = m_glyphBaseFromTop + glyph.yOffset -1;
        std::cout << "curY: " << int(curY) << "\n";

        // !! TODO: Change the ContentData struct to provide the size of the mask to prevent
        // the magic numbers in this loop.

        // Fill the data struct row-by-row
        for(uint16_t i = glyph.bitmapOffset; i < (glyph.bitmapOffset + numBytesFromTableNeeded); i++) {
            // !! TODO: Implement something to indicate the overlap of a character on two separate contectStructs
            
            uint8_t charByte = m_activeFont->bitmap[i];
            std::cout << "Current charByte: " << std::bitset<8>(charByte) << " with index: " << i << "\n";

            std::cout << "overlapBitsRemaining: " << int(overlapBitsRemaining) << "\n";
            if(overlapBitsRemaining != 0) {
                // Create a bitmask for the character width
                uint8_t charByteActivePart = 0x0;
                for(uint8_t p = 0; p < overlapBitsRemaining; p++) {
                    charByteActivePart |= (0x1 << p);
                }
                std::cout << "charByteActivePart overlapping: " << std::bitset<8>(charByteActivePart) << "\n";

                // Add the character data to the contentStruct
                std::cout << "Adding overlapping charByte to row: " << int(curY) << "\n";
                contentStructToFill.contentMask[curY] |= ((charByte & charByteActivePart)  << (31 - cursorX - (glyph.width-overlapBitsRemaining)));
                std::cout << "contentStructToFill.contentMask[curY] overlapping: " << std::bitset<32>(contentStructToFill.contentMask[curY]) << "\n";
                curY++;
                overlapBitsRemaining = 0;
            }
            
            // A single byte can possibly contain data for multiple row if the with of the character is less than 8.
            uint8_t numRowsInOneByte = ceil(8.0/glyph.width);
            for(int8_t w = numRowsInOneByte-1; w >= 0; w--) {
                // Create a bitmask for the character width
                uint8_t charByteActivePart = 0x0;
                std::cout << "glyph.width: " << int(glyph.width) << "\n";
                for(uint8_t p = 0; p < glyph.width && p < 8; p++) {
                    charByteActivePart |= (0x1 << p);
                    std::cout << "charByteActivePart internal: " << std::bitset<8>(charByteActivePart) << "\n";
                }
                if(glyph.width < 8) {
                    charByteActivePart <<= (w*glyph.width)-1;
                }
                std::cout << "charByteActivePart: " << std::bitset<8>(charByteActivePart) << "\n";

                // Add the character data to the contentStruct
                std::cout << "Adding charByte to row: " << int(curY) << "\n";
                std::cout << "Shifting data with " << int(31 - cursorX - glyph.width) << " points\n";
                // !! NOTE: the (31 - ) wont work for overlapping contentStructs if the cursor would keep increasing
                contentStructToFill.contentMask[curY] |= ((charByte & charByteActivePart)  << (31 - cursorX - glyph.width));
                std::cout << "contentStructToFill.contentMask[curY]: " << std::bitset<32>(contentStructToFill.contentMask[curY]) << "\n";

                if((w > 0) || ((w == 0) && (8 % glyph.width) == 0)) {
                    curY++;
                } else {
                    overlapBitsRemaining = glyph.width - (8 % glyph.width);
                }
            }
        }

        cursorX += glyph.xAdvance + glyph.xOffset;

        std::cout << "glyph.xAdvance: " << int(glyph.xAdvance) << std::endl;
        std::cout << "glyph.xOffset: " << int(glyph.xOffset) << std::endl;
        std::cout << "new cursorX: " << int(cursorX) << std::endl;
    }

    contentStructToFill.width = cursorX;

    return success;
}