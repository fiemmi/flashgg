queue="workday"
fggRunJobs.py --load UL_runII_v1_2017.json -d DataUL2017_4Untag_retrainBDT_updatedUncs  -x cmsRun workspaceStd_OnlyUntag.py maxEvents=-1 -q ${queue} -n 200 --no-copy-proxy --no-use-tarball  dumpWorkspace=False dumpTrees=True 
