#pragma once

#include <stdint.h>
#include <string>
#include <ledMatrixDriver/displayContent/ContentData.h>

class ImageProvider {
    public:

        // TODO: make something which takes into account content overflow in the Y axis

        // If getImage() returns true, the caller of getImage() should call getImage() again with
        // the contentOverflow parameter set to true and the other parameter unchanged.
        // getImage() will then fill the contentStructToFill with the rest of the text.
        // This process can be repeated untill getImage() return false. Indicating no other passes are needed
        bool getImage(std::string imageID, ContentData& contentStructToFill, bool contentOverflow = false);
    
    private:
        int16_t m_cursorX = 0;
};