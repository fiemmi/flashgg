queue="workday"

fggRunJobs.py --load DoubleEG_PostVFP_Summer20UL.json -d DataUL2016_postVPF_4Untag_nosigmarv  -x cmsRun workspaceStd_OnlyUntag.py maxEvents=-1 -q ${queue} -n 100 --no-copy-proxy --no-use-tarball  dumpWorkspace=False dumpTrees=True

