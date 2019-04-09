#include "path_utils.h"

namespace Retro3D
{
    std::string PathUtils::CombinePaths(const std::string& a, const std::string& b)
    {
        if (a == "")
            return b;
        else
        {
            return a + "/" + b; // TODO
        }
    }
}
