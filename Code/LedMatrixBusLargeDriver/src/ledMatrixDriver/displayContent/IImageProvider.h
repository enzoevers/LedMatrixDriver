#pragma once

#include <stdint.h>
#include <string>
#include <ledMatrixDriver/displayContent/ContentData.h>

class IImageProvider {
  public:
    virtual ~IImageProvider();

    // The return value of this function indicated how many structs are left to get
    // in order to get the complete image.
    //
    // !! TODO: Specify the order of image part retrieval
    virtual uint8_t getImage(std::string imageID, ContentData& contentStructToFill) = 0;
};
