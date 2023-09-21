#include "flashgg/DataFormats/interface/BothEBUntaggedTag.h"

using namespace flashgg;

BothEBUntaggedTag::BothEBUntaggedTag() : DiPhotonTagBase::DiPhotonTagBase() {}

BothEBUntaggedTag::~BothEBUntaggedTag() {}

BothEBUntaggedTag::BothEBUntaggedTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes )
    : BothEBUntaggedTag::BothEBUntaggedTag( diPho, *mvaRes ) {}

BothEBUntaggedTag::BothEBUntaggedTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :  DiPhotonTagBase::DiPhotonTagBase( dipho,
            mvares ) {}

BothEBUntaggedTag *BothEBUntaggedTag::clone() const
{
    BothEBUntaggedTag *result = new BothEBUntaggedTag( *this );
    return result;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

