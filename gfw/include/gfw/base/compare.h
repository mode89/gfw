#ifndef __GFW_BASE_COMPARE_H__
#define __GFW_BASE_COMPARE_H__

namespace GFW {

    enum CompareFunc
    {
        COMPARE_NEVER,          ///< Never pass the comparison.
        COMPARE_LESS,           ///< If the source data is less than the destination data, the comparison passes.
        COMPARE_EQUAL,          ///< If the source data is equal to the destination data, the comparison passes.
        COMPARE_LESS_EQUAL,     ///< If the source data is less than or equal to the destination data, the comparison passes.
        COMPARE_GREATER,        ///< If the source data is greater than the destination data, the comparison passes.
        COMPARE_NOT_EQUAL,      ///< If the source data is not equal to the destination data, the comparison passes.
        COMPARE_GREATER_EQUAL,  ///< If the source data is greater than or equal to the destination data, the comparison passes.
        COMPARE_ALWAYS,         ///< Always pass the comparison.
    };

} // namespace GFW

#endif // __GFW_BASE_COMPARE_H__
