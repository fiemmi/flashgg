# JTao - lxplus
#espresso, microcentury, longlunch, workday, tomorrow, testmatch, nextweek
#queue="longlunch"
queue="microcentury"  #"longlunch"
#
fggRunJobs.py --load sig_UL2017.json -d Sig125_MiniTree -x cmsRun workspaceStd_Tree.py maxEvents=100 -n 100 -q ${queue} -H -D -P  --no-use-tarball  --no-copy-proxy dumpWorkspace=False dumpTrees=True  doSystematics=False  MiniTreeOnly=True 
 

