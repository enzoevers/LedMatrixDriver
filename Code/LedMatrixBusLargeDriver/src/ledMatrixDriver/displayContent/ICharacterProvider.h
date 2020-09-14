#pragma once

#include <stdint.h>
#include <ledMatrixDriver/displayContent/ContentData.h>

class ICharacterProvider {
  public:
    virtual ~ICharacterProvider();

    //
    // https://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=IWS-Chapter08#:~:text=A%20TrueType%20font%20is%20a%20binary%20file%20containing%20a%20number%20of%20tables.&text=Each%20table%20and%20the%20whole,smart%20rendering%20and%20PostScript%20glyphs.
    // https://github.com/k-omura/truetype_Arduino
    // https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6EBSC.html
    // https://docs.microsoft.com/en-us/typography/opentype/spec/ttch01
    // https://learnopengl.com/In-Practice/Text-Rendering
    // https://stackoverflow.com/questions/2672144/dump-characters-glyphs-from-truetype-font-ttf-into-bitmaps


    // The characterID is used identify the character/image to retreive.
    // Since the data in the Character struct can contain any data, it can also 
    // contain (part of) a logo for instance.
    // 
    // The characterID string is constructed like this:
    //
    //  /Characters (Contains several fonts: https://www.dafont.com/bitmap.php)
    //      /04b_30 (https://www.dafont.com/04b-30.font?fpp=200)
    //      /Baby_Blocks (https://www.dafont.com/babyblocks.font?back=bitmap&fpp=200)
    //      /8_Bit_Wonder (https://www.dafont.com/8bit-wonder.font?fpp=200)
    //      /Rainy_Hearts (https://www.dafont.com/rainyhearts.font?fpp=200)
    //      /Jorolks (https://www.dafont.com/jorolks.font?fpp=200)        
    //      /Vermin_Vibes 1989 (https://www.dafont.com/vermin-vibes-1989.font?fpp=200)
    //      /Early_GameBoy (https://www.dafont.com/early-gameboy.font?fpp=200)
    //          /32.txt (all the ASCII + Extended ASCII values: https://theasciicode.com.ar/)
    //          /33.txt
    //          :
    //          /255.txt
    //  /Images
    //      /Logo
    //          /Arduino_small.txt
    //          /Arduino_large (when a logo is smaller than the maximum buffer size it should be split into multiple files)
    //              /0.txt
    //              /1.txt
    //      /Emoji
    //          /Happy_parentheses.txt
    //          /Happy_c.txt
    //          /Sad_parentheses.txt
    //          /Sad_c.txt
    //      /Misc
    //          /Kurzgesacht_bird.txt
    //          /Cube.txt
    //          /Circle.txt





    // The characterID is a uint16_t to support ASCII extents
    virtual bool getCharacter(uint16_t characterID, ContentData& contentStructToFill) = 0;
};
