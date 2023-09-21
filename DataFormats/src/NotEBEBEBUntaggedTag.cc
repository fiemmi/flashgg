#include "flashgg/DataFormats/interface/NotEBEBUntaggedTag.h"

using namespace flashgg;

NotEBEBUntaggedTag::NotEBEBUntaggedTag() : DiPhotonTagBase::DiPhotonTagBase() {}

NotEBEBUntaggedTag::~NotEBEBUntaggedTag() {}

NotEBEBUntaggedTag::NotEBEBUntaggedTag( edm::Ptr<flashgg::DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvaRes )
    : NotEBEBUntaggedTag::NotEBEBUntaggedTag( diPho, *mvaRes ) {}

NotEBEBUntaggedTag::NotEBEBUntaggedTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) :  DiPhotonTagBase::DiPhotonTagBase( dipho,
            mvares ) {}

NotEBEBUntaggedTag *NotEBEBUntaggedTag::clone() const
{
    NotEBEBUntaggedTag *result = new NotEBEBUntaggedTag( *this );
    return result;
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

