#ifndef __GFW_PIPELINE_OGL4_VALIDATOR_H__
#define __GFW_PIPELINE_OGL4_VALIDATOR_H__

namespace GFW {

    void
    AcquireValidator();

    void
    ReleaseValidator();

    void
    Validate( const char * source );

} // namespace GFW

#endif // __GFW_PIPELINE_OGL4_VALIDATOR_H__
