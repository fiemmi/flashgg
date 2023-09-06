# JTao - lxplus
#espresso, microcentury, longlunch, workday, tomorrow, testmatch, nextweek
queue="workday"  #"longlunch"
#
fggRunJobs.py --load Sig120_postVFP_Summer20UL.json -d Sig120_2016PostVFP_nosigmarv -x cmsRun workspaceStd_OnlyUntag.py maxEvents=-1 -n 200 -q ${queue} -H -D -P  --no-use-tarball  --no-copy-proxy  dumpWorkspace=False dumpTrees=True

 

