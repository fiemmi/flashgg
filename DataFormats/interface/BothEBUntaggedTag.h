#ifndef flashgg_BothEBUntaggedTag
#define flashgg_BothEBUntaggedTag

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"

namespace flashgg {

    class BothEBUntaggedTag: public DiPhotonTagBase
    {
    public:
        BothEBUntaggedTag();
        ~BothEBUntaggedTag();

        BothEBUntaggedTag( edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult );
        BothEBUntaggedTag( edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult> );
        virtual BothEBUntaggedTag *clone() const override;
        DiPhotonTagBase::tag_tao mytagEnum() const override {return DiPhotonTagBase::kBothEBUntagged; }

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

