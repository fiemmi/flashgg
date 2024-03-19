#include "flashgg/DataFormats/interface/Photon.h"
//#include "flashgg/Systematics/interface/ObjectSystMethodBinnedByFunctor.h"
#include "flashgg/Systematics/interface/BaseSystMethod.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "RecoEgamma/EgammaTools/interface/EnergyScaleCorrection.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "flashgg/Systematics/src/FNUFScaleCorrection.cc"

namespace edm {
    class Event;
}

namespace flashgg {

    //class PhotonFNUFScaleEGMTool: public ObjectSystMethodBinnedByFunctor<flashgg::Photon, int>
    class PhotonFNUFScaleEGMTool: public BaseSystMethod<flashgg::Photon, int>
    {

    public:
        typedef StringCutObjectSelector<Photon, true> selector_type;

        PhotonFNUFScaleEGMTool( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv );
        void applyCorrection( flashgg::Photon &y, int syst_shift ) override;
        std::string shiftLabel( int ) const override;
        void eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) override;
        //FNUFScaleCorrection FNUFscaler;
        //FNUFScaleCorrection FNUFscaler(std::string RR0maps_name, std::string models_name);
    private:
        selector_type overall_range_;
        edm::FileInPath modelFile_;
        edm::FileInPath laserResponseFile_;
        double r9threshold_;
        double muNatural_corr_;
        double fractional_unc_;
        std::bitset<EnergyScaleCorrection::kErrNrBits> uncBitMask_; //FIemmi: inherited but not actually used
        bool debug_;
        unsigned run_number_;
        FNUFScaleCorrection FNUFscaler;
    };

    PhotonFNUFScaleEGMTool::PhotonFNUFScaleEGMTool( const edm::ParameterSet &conf, edm::ConsumesCollector && iC, const GlobalVariablesComputer *gv ) :
        BaseSystMethod( conf, std::forward<edm::ConsumesCollector>(iC)  ),
        overall_range_( conf.getParameter<std::string>( "OverallRange" ) ),
        modelFile_( conf.getParameter<edm::FileInPath>( "ModelFile" )),
        laserResponseFile_( conf.getParameter<edm::FileInPath>( "LaserResponseFile" )),
        r9threshold_( conf.getParameter<double>( "R9threshold" )),
        muNatural_corr_( conf.getParameter<double>( "MuNatural_corr" )),
        fractional_unc_( conf.getParameter<double>( "FractionalUncertainty" )),
        uncBitMask_( conf.getParameter<std::string>("UncertaintyBitMask" ) ), //FIemmi: not needed
        debug_( conf.getUntrackedParameter<bool>( "Debug", false ) ),
        //FNUFscaler("/afs/cern.ch/work/f/fiemmi/private/Hgg/MassMeasurement/CMSSW_10_6_29/src/flashgg/Systematics/test_data/average_RoverR0_2016_preVFP.root", "/afs/cern.ch/work/f/fiemmi/private/Hgg/MassMeasurement/CMSSW_10_6_29/src/flashgg/Systematics/test_data/231024_HggMass_distributions.root")
        FNUFscaler(laserResponseFile_.fullPath().c_str(), modelFile_.fullPath().c_str())
    {
  
        //Create a FNUF scaler object
        //FNUFscaler("/afs/cern.ch/work/f/fiemmi/private/Hgg/MassMeasurement/CMSSW_10_6_29/src/flashgg/Systematics/test_data/average_RoverR0_2016_preVFP.root", "/afs/cern.ch/work/f/fiemmi/private/Hgg/MassMeasurement/CMSSW_10_6_29/src/flashgg/Systematics/test_data/231024_HggMass_distributions.root");
        
    }

    void PhotonFNUFScaleEGMTool::eventInitialize( const edm::Event &iEvent, const edm::EventSetup & iSetup ) {
        run_number_ = iEvent.run();
    }
    
    std::string PhotonFNUFScaleEGMTool::shiftLabel( int syst_value ) const
    {
        std::string result;
        if( syst_value == 0 ) {
            result = Form( "%sCentral", label().c_str() );
        } else if( syst_value > 0 ) {
            result = Form( "%sUp%.2dsigma", label().c_str(), syst_value );
        } else {
            result = Form( "%sDown%.2dsigma", label().c_str(), -1 * syst_value );
        }
        return result;
    }

    void PhotonFNUFScaleEGMTool::applyCorrection( flashgg::Photon &y, int syst_shift )
    {
        if( overall_range_( y ) ) {
            auto shift_val = FNUFscaler.scaleCorr(run_number_, y.superCluster()->eta(), y.full5x5_r9(), y.energy(), r9threshold_, muNatural_corr_, false);
            auto shift_err = FNUFscaler.scaleCorrUncert(shift_val, fractional_unc_);
            if (!applyCentralValue()) shift_val = 1.;
            float scale = shift_val + syst_shift * shift_err;
            if( debug_ ) {
                std::cout << "TESTING FNUF CORRECTION " << shiftLabel( syst_shift ) << std::endl;
                std::cout << FNUFscaler.scaleCorr(run_number_, y.superCluster()->eta(), y.full5x5_r9(), y.energy(), r9threshold_, muNatural_corr_, debug_) << std::endl;
            }
            y.updateEnergy( shiftLabel( syst_shift ), scale * y.energy() );
        }
    }
}

DEFINE_EDM_PLUGIN( FlashggSystematicPhotonMethodsFactory,
                   flashgg::PhotonFNUFScaleEGMTool,
                   "FlashggPhotonFNUFScaleEGMTool" );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
