#pragma once

#include <stdint.h>
#include <string>
#include <ledMatrixDriver/displayContent/ContentData.h>
#include <Adafruit_GFX/gfxfont.h>

class ICharacterProvider {
  public:
    virtual ~ICharacterProvider() {};


    virtual bool setFont(GFXfont& font) =0;

    // If getText() returns true, the caller of getText() should call getText() again with
    // the contentOverflow parameter set to true and the other parameter unchanged.
    // getText() will then fill the contentStructToFill with the rest of the text.
    // This process can be repeated untill getText() return false. Indicating no other passes are needed
    virtual bool getText(std::string text, ContentData& contentStructToFill, bool contentOverflow = false) = 0;
};
