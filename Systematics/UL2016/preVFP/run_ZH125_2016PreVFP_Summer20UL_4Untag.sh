# JTao - lxplus
#espresso, microcentury, longlunch, workday, tomorrow, testmatch, nextweek
queue="workday"  #"longlunch"
#
fggRunJobs.py --load ZH125_preVFP_Summer20UL.json -d ZH125_2016PreVFP_Summer20UL -x cmsRun workspaceStd_OnlyUntag.py maxEvents=-1 -n 200 -q ${queue} -H -D -P  --no-use-tarball  --no-copy-proxy  dumpWorkspace=False dumpTrees=True  MiniTreeOnly=True

 

