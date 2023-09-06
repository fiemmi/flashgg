# JTao - lxplus
#espresso, microcentury, longlunch, workday, tomorrow, testmatch, nextweek
#queue="longlunch"
queue="workday"  #"longlunch"
#
fggRunJobs.py --load sig130_UL2017.json -d Sig130_4Unatg_retrainBDT_final -x cmsRun workspaceStd_OnlyUntag.py maxEvents=-1 -n 100 -q ${queue} -H -D -P  --no-use-tarball  --no-copy-proxy dumpWorkspace=False dumpTrees=True  doSystematics=True useParentDataset=False doPdfWeights=False

#FIemmi: set useParentDataset to False if you don't want to load the parent MiniAOD file to recalcuate the thoery uncertainties 
#Fiemmi: set doSystematics=False if you don't care about the systematic part (e.g., you only want to check the statystical error on the Higgs mass) 
#Fiemmi: set copyInputMicroAOD=1. Instead of doing sth like root -l root://cms-xrd-global.cern.ch///path/to/file.root, do instead xrdcp root://cms-xrd-global.cern.ch///path/to/file.root ./file.root; root -l ./file.root
