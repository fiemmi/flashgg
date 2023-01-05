# JTao - lxplus
#espresso, microcentury, longlunch, workday, tomorrow, testmatch, nextweek
#queue="longlunch"
queue="workday"  #"longlunch"
#
fggRunJobs.py --load zh_UL2017.json -d ZH125_4Unatg_retrainBDT_updatedUncs -x cmsRun workspaceStd_OnlyUntag.py maxEvents=-1 -n 100 -q ${queue} -H -D -P  --no-use-tarball  --no-copy-proxy dumpWorkspace=False dumpTrees=True  doSystematics=True useParentDataset=False doPdfWeights=False
 
 

