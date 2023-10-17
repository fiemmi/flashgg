queue="workday"
fggRunJobs.py --load UL_runII_v1_2018_Summer20.json -d DataUL2018_4Untag_retrainBDT_MassWindow_testSummer20samples -x cmsRun workspaceStd_OnlyUntag.py maxEvents=-1 -q ${queue} -n 200 --no-copy-proxy --no-use-tarball  dumpWorkspace=False dumpTrees=True 
