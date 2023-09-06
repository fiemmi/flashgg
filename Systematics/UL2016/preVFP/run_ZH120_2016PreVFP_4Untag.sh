# JTao - lxplus
#espresso, microcentury, longlunch, workday, tomorrow, testmatch, nextweek
queue="workday"  #"longlunch"
#
fggRunJobs.py --load ZH120_preVFP_Summer20UL.json -d ZH120_2016PreVFP_nosigmarv -x cmsRun workspaceStd_OnlyUntag.py maxEvents=-1 -n 200 -q ${queue} -H -D -P  --no-use-tarball  --no-copy-proxy  dumpWorkspace=False dumpTrees=True doSystematics=True useParentDataset=False doPdfWeights=False

 

