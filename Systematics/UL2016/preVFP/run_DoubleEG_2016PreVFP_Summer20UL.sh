queue="workday"

fggRunJobs.py --load DoubleEG_2016PreVFP_Summer20UL.json -d DoubleEG_2016PreVFP_Summer20UL_Minitree  -x cmsRun workspaceStd_Tree.py maxEvents=-1 -q ${queue} -n 100 --no-copy-proxy --no-use-tarball  dumpWorkspace=False dumpTrees=True MiniTreeOnly=True

