queue="workday"

fggRunJobs.py --load UL_runII_v1_2017.json -d DataUL2017_Minitree  -x cmsRun workspaceStd_Tree.py maxEvents=-1 -q ${queue} -n 100 --no-copy-proxy --no-use-tarball  dumpWorkspace=False dumpTrees=True MiniTreeOnly=True

