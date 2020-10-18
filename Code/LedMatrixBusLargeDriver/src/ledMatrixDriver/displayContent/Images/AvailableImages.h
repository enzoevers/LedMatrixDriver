#include <string>
#include <ledMatrixDriver/displayContent/Images/ImageMeta.h>

using imageKeyType = const std::string;
using imageDataType = const ImageMeta;

extern std::unordered_map<imageKeyType, imageDataType> imageMap;