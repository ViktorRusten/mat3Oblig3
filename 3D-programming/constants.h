#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace gsl
{
const std::string assetFilePath{"../3D-Programming/assets/"};
const std::string shaderFilePath{"../3D-Programming/"};

enum ShaderTypes
{
    COLOR,
    TEXTURE,
    COLORLIGHT,
    TEXTURELIGHT
};
}
#endif // CONSTANTS_H
