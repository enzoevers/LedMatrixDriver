#include <ledMatrixDriver/displayContent/ImageProvider.h>
#include <ledMatrixDriver/displayContent/Images/AvailableImages.h>

#include  <stdexcept>

bool ImageProvider::getImage(const std::string imageID, ContentData& contentStructToFill, bool contentOverflow)
{
    bool needExtraPass = true;
    bool doLastPartt = false;

    if(contentOverflow == false) {
        m_cursorX = 0;
    } else {
        m_cursorX -= contentStructToFill.maxRowWidth;
    }

    contentStructToFill.clearData();

    try {
        imageDataType& imageData = imageMap.at(imageID);
        contentStructToFill.height = imageData.height;
    } catch (std::out_of_range) {
        return false;
    }

    bool needExtraContectStructForText = false;

    // TODO: To be adapted for the image provider

    /*
    for(uint16_t c = m_currentTextIndex; c < m_currentText.length(); c++) {
        uint16_t glyphIndex = m_currentText.at(c) - m_activeFont->first;
        GFXglyph glyph = m_activeFont->glyph[glyphIndex];

        const uint16_t numPixelsNedded = glyph.height * glyph.width;

        // Dividing by 8 bits. The .0 is to make it a float which makes it possible to
        // use the ceil() function.
        const uint8_t numBytesFromTableNeeded = ceil((numPixelsNedded) / 8.0);

        std::vector<uint8_t> pixelArray = {};
        
        // The glyph.yOffset if negative for most 'normal' characters
        int8_t curY = m_glyphBaseFromTop + glyph.yOffset;

        if(c == 0 && contentOverflow == false) {
            m_cursorX -= glyph.xOffset; // To save some space on the left
        }

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
                    contentStructToFill.contentMask[curY] |= ((uint64_t)0x1 << ((contentStructToFill.maxRowWidth-1) - m_cursorX - glyph.xOffset - x));
                }                
            }   
            curY++;
            if(curY == contentStructToFill.maxRows) {
                curY = contentStructToFill.maxRows;
            }
        }

        m_cursorX += glyph.xAdvance;   
        
        if(m_cursorX < contentStructToFill.maxRowWidth && c == m_currentText.length()-1) {
            needExtraPass = false;
            contentStructToFill.width = m_cursorX+1; // m_cursorX is a zero-based index, thus the +1.
        } else if (m_cursorX >= contentStructToFill.maxRowWidth) {
            // Either part of the current character is already written to the struct and the last part
            // needs to be written on the next struct
            if((m_cursorX - contentStructToFill.maxRowWidth) < glyph.xAdvance){
                contentStructToFill.width = contentStructToFill.maxRowWidth;
                m_cursorX -= glyph.xAdvance; // In order to only get the last right part of the character on the next struct
                m_currentTextIndex = c; // The rest of this character should be handled on the next call to this function
                break;
            }
            
            // Or the next character has a negative xOffset which means
            // a part of the character is still in this content struct.
            uint16_t local_glyphIndex = m_currentText.at(c+1) - m_activeFont->first;
            GFXglyph local_glyph = m_activeFont->glyph[local_glyphIndex];

            if(local_glyph.xOffset >= 0 || ((m_cursorX + local_glyph.xOffset) - contentStructToFill.maxRowWidth) >= 0) {
                contentStructToFill.width = contentStructToFill.maxRowWidth;
                break; // In these cases the complete next character is on the next datastruct. So there is no problem.
            } else {
                if(doLastCharacter) {
                    contentStructToFill.width = contentStructToFill.maxRowWidth;
                    break;
                }
                doLastCharacter = true;
                m_currentTextIndex = c; // The rest of this character should be handled on the next call to this function
            }            
        }
    }

    return needExtraPass;
    */
}