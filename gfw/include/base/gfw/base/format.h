#ifndef __GFW_BASE_FORMAT_H__
#define __GFW_BASE_FORMAT_H__

namespace GFW {

    enum Type
    {
        TYPE_UNKNOWN = 0,
        TYPE_FLOAT,
        TYPE_USHORT,
        TYPE_UINT,
        TYPE_UBYTE,
    };

    enum Format
    {
        FORMAT_UNKNOWN = 0,
        FORMAT_RGB32_FLOAT,
        FORMAT_RG32_FLOAT,
        FORMAT_RGBA8_UNORM,
    };

} // namespace GFW

#endif // __GFW_BASE_FORMAT_H__
