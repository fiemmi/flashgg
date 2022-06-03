import FWCore.ParameterSet.Config as cms

class UntagMiniTreeCustomize():
    """
    Customizaton class for MiniTree analysis
    """

    def __init__(self, process, customize, metaConditions):
        self.process = process
        self.customize = customize
        self.metaConditions = metaConditions
        self.tagList = [
           ["LOGICERROR",0], ["NOTAG",0],
           ["UntaggedTag",0]
        ]

    def variablesToDump(self):
        ws_variables = []
        ws_variables += [
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass",
            "dZ[40,-20.,20.]:=(tagTruth().genPV().z-diPhoton().vtx().z)",
            #            "btagReshapeNorm_TTH_LEP[1,-999999.,999999.] := weight(\"btagReshapeNorm_TTH_LEP\")",
            #            "btagReshapeNorm_TTH_HAD[1,-999999.,999999.] := weight(\"btagReshapeNorm_TTH_HAD\")",
            #            "btagReshapeNorm_THQ_LEP[1,-999999.,999999.] := weight(\"btagReshapeNorm_THQ_LEP\")",
            "diphoMVA[200,-1,1] :=diPhotonMVA().result",
            "leadR9[150,0,1.5]  :=diPhoton().leadingPhoton.full5x5_r9",
            "subleadR9[150,0,1.5] :=diPhoton().subLeadingPhoton.full5x5_r9",
            "leadphi          := diPhoton().leadingPhoton.superCluster.phi",
            "subleadphi       := diPhoton().subLeadingPhoton.superCluster.phi",
            "leadptom         := diPhotonMVA.leadptom  ",
            "subleadptom      := diPhotonMVA.subleadptom ",
            "leadmva          := diPhotonMVA.leadmva ",
            "subleadmva       := diPhotonMVA.subleadmva    ",
            "leadeta          := diPhotonMVA.leadeta     ",
            "subleadeta       := diPhotonMVA.subleadeta",
            "sigmarv          := diPhotonMVA.sigmarv",
            "sigmarvDecorr    := diPhotonMVA.decorrSigmarv",
            "sigmawv          := diPhotonMVA.sigmawv",
            "CosPhi           := diPhotonMVA.CosPhi",
            "vtxprob          := diPhotonMVA.vtxprob",
            "pt               := diPhoton.pt",
            "leadSigEOverE    := diPhoton.leadingPhoton().sigEOverE()",
            "subleadSigEOverE := diPhoton.subLeadingPhoton().sigEOverE()",
            "lead_genMatchType :=diPhoton().leadingPhoton.genMatchType",
            "sublead_genMatchType :=diPhoton().subLeadingPhoton.genMatchType",
            ##
            "weight_electronVetoSF[1,-999999.,999999.]:=weight(\"electronVetoSFCentral\")",
            "weight_PreselSF[1,-999999.,999999.]:=weight(\"PreselSFCentral\")",
            "weight_TriggerWeight[1,-999999.,999999.]:=weight(\"TriggerWeightCentral\")",
            "weight_LooseMvaSF[1,-999999.,999999.]:=weight(\"LooseMvaSFCentral\")",
            "weight_FracRVWeight[1,-999999.,999999.]:=weight(\"FracRVWeightCentral\")",
            "weight_FracRVNvtxWeight[1,-999999.,999999.]:=weight(\"FracRVNvtxWeightCentral\")",
            "weight_SigmaEOverESmearing[1,-999999.,999999.]:=weight(\"SigmaEOverESmearingCentral\")",
            ##
            "NNLOPSweight[1,-999999.,999999.] := tagTruth().weight(\"NNLOPSweight\")",
            "centralObjectWeight[1,-999999.,999999.] := centralWeight"
        ]
        return ws_variables

    def DataVariablesToDump(self):
        ws_variables = []
        ws_variables += [
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass",
            "dZ  := ?!tagTruth().isNull()?abs(tagTruth().genPV().z-diPhoton().vtx().z):-99.",
            ##
            "diphoMVA[200,-1,1] :=diPhotonMVA().result",
            "leadR9[150,0,1.5]  :=diPhoton().leadingPhoton.full5x5_r9",
            "subleadR9[150,0,1.5] :=diPhoton().subLeadingPhoton.full5x5_r9",
            "leadphi          := diPhoton().leadingPhoton.superCluster.phi",
            "subleadphi       := diPhoton().subLeadingPhoton.superCluster.phi",
            "leadptom         := diPhotonMVA.leadptom  ",
            "subleadptom      := diPhotonMVA.subleadptom ",
            "leadmva          := diPhotonMVA.leadmva ",
            "subleadmva       := diPhotonMVA.subleadmva    ",
            "leadeta          := diPhotonMVA.leadeta     ",
            "subleadeta       := diPhotonMVA.subleadeta",
            "sigmarv          := diPhotonMVA.sigmarv",
            "sigmarvDecorr    := diPhotonMVA.decorrSigmarv",
            "sigmawv          := diPhotonMVA.sigmawv",
            "CosPhi           := diPhotonMVA.CosPhi",
            "vtxprob          := diPhotonMVA.vtxprob",
            "pt               := diPhoton.pt",
            "leadSigEOverE    := diPhoton.leadingPhoton().sigEOverE()",
            "subleadSigEOverE := diPhoton.subLeadingPhoton().sigEOverE()",
            "lead_genMatchType :=diPhoton().leadingPhoton.genMatchType",
            "sublead_genMatchType :=diPhoton().subLeadingPhoton.genMatchType",
            ##
            "centralObjectWeight[1,-999999.,999999.] := centralWeight"
        ]
        return ws_variables


    def systematicVariables(self):
        systematicVariables = []
        systematicVariables += [
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass"
        ]
        return systematicVariables

