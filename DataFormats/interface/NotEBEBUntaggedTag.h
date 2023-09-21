#ifndef flashgg_NotEBEBUntaggedTag
#define flashgg_NotEBEBUntaggedTag

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

    class NotEBEBUntaggedTag: public DiPhotonTagBase
    {
    public:
        NotEBEBUntaggedTag();
        ~NotEBEBUntaggedTag();

        NotEBEBUntaggedTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        NotEBEBUntaggedTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        virtual NotEBEBUntaggedTag *clone() const override;
        DiPhotonTagBase::tag_tao mytagEnum() const override {return DiPhotonTagBase::kNotEBEBUntagged; }

    private:



    };
}

#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

