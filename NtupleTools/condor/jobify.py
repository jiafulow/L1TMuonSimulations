#!/usr/bin/env python

"""
A condor job submitter. This script prepares the input source file, the job
config, and the executable.
"""

import os
import sys
import subprocess

class CondorJobType(object):

    def __init__(self):

        self.projdir        = 'blt_projects'
        self.srcdir         = 'sourceFiles'
        self.pset_name      = 'PSet.py'
        self.tgz_name       = 'default.tgz'
        self.txt_name       = 'input.txt'
        self.jdl_name       = 'blt.jdl'
        self.exe_name       = 'blt.sh'
        self.chk_name       = '.checkfile.txt'
        self.jobid          = 1

        self.analyzer       = sys.argv[1]
        self.dataset        = sys.argv[2]
        self.datasetgroup   = sys.argv[3]
        self.selection      = sys.argv[4]
        self.period         = sys.argv[5]
        self.njobs          = int(sys.argv[6])

        self.config = {
            'PROJDIR'      : self.projdir,
            'SRCDIR'       : self.srcdir,
            'PSETFILE'     : self.pset_name,
            'TARBALL'      : self.tgz_name,
            'SOURCEFILE'   : self.txt_name,
            'JOBAD'        : self.jdl_name,
            'EXECUTABLE'   : self.exe_name,
            'CHECKFILE'    : self.chk_name,
            'JOBID'        : self.jobid,

            'ANALYZER'     : self.analyzer,
            'DATASET'      : self.dataset,
            'DATASETGROUP' : self.datasetgroup,
            'SELECTION'    : self.selection,
            'PERIOD'       : self.period,
            'NJOBS'        : self.njobs,

            'MAXEVENTS'    : '-1',
            'JOBNAME'      : 'job',
            'LOGNAME'      : 'res',
            'OUTNAME'      : 'out',
        }

        self.config['JOBPATH'] = '{PROJDIR}/{ANALYZER}/{DATASET}'.format(**self.config)

        hostname = subprocess.check_output(['hostname']).rstrip('\n')
        if hostname.endswith('fnal.gov'):
            machine = 'fnal.gov'
            storage = 'srm://cmseos.fnal.gov:8443/srm/v2/server?SFN=/eos/uscms/store/group/l1upgrades/L1MuonTrigger/'
        elif hostname.endswith('ufl.edu'):
            machine = 'ufl.edu'
            storage = 'srm://srm.ihepa.ufl.edu:8443/srm/v2/server?SFN=/cms/data/store/user/jiafulow/L1MuonTrigger/'
        else:
            machine = 'unknown'
            storage = 'srm://unknown/'
        self.config['MACHINE'] = machine
        self.config['STORAGE'] = storage + '{JOBPATH}'.format(**self.config)
        self.config['STORAGE2'] = storage[storage.find('SFN=')+4:] + '{JOBPATH}'.format(**self.config)
        self.config['TARBALL2'] = storage + '{PROJDIR}/{TARBALL}'.format(**self.config)

        self.config['CMSSW_VERSION'] = os.environ['CMSSW_VERSION']
        self.config['SCRAM_ARCH'] = os.environ['SCRAM_ARCH']

        #cp_cmd = "lcg-cp -v -b -D srmv2 --connect-timeout 180 "
        #delfile_cmd = "lcg-del --connect-timeout 180 -b -l -D srmv2 "
        #deldir_cmd = "lcg-del -d --connect-timeout 180 -b -l -D srmv2 "

    def safety_check(self):
        if not os.path.exists('{SRCDIR}/{DATASET}.txt'.format(**self.config)):
            raise Exception('Cannot find source file: {SRCDIR}/{DATASET}.txt'.format(**job.configurations))

        if not os.path.exists('{TARBALL}'.format(**self.config)):
            raise Exception('Cannot find tarball: {TARBALL}'.format(**self.config))

    def run(self):
        self.make_dirs()

        self.write_exe()

        while self.jobid <= self.njobs:
            if self.jobid == 1:
                self.write_jdl()
            else:
                self.append_jdl()
            self.jobid += 1
            self.config['JOBID'] = self.jobid

        self.upload_tarball()

        self.add_check()

        self.submit_jobs()
        return

    def execute_commands(self, commands):
        commands = commands.strip().split('\n')
        for cmd in commands:
            subprocess.check_call(cmd, shell=True)
        return

    def make_dirs(self):
        commands = \
'''mkdir -p {JOBPATH}/
rm -rf {JOBPATH}/*
mkdir {JOBPATH}/{JOBNAME}/ {JOBPATH}/{LOGNAME}/
ln -s {STORAGE2} {JOBPATH}/{OUTNAME}
cp {SRCDIR}/{DATASET}.txt {SOURCEFILE}
'''.format(**self.config)
        self.execute_commands(commands)
        return

    def write_jdl(self):

        if 'X509_USER_PROXY' not in os.environ:
            myproxy = '/tmp/x509up_u%s' % (subprocess.check_output(['id', '-u']).rstrip('\n'))
            os.environ['X509_USER_PROXY'] = myproxy

        # NOTE:
        # on fnal.gov, uncomment 'Universe = vanilla', comment out 'Universe = grid' and 'grid_resource = ...'.
        machine = self.config['MACHINE']
        if machine == "fnal.gov":
            print "[NOTE   ] Make sure to uncomment 'Universe = vanilla', comment out 'Universe = grid' and 'grid_resource = ...'."

        writeme = \
'''#Universe                = vanilla
Universe                = grid
grid_resource           = condor osg.rc.ufl.edu osg.rc.ufl.edu:9619
#grid_resource           = condor red-gw1.unl.edu red-gw1.unl.edu:9619
Notification            = Error
Executable              = {JOBNAME}/{EXECUTABLE}
Arguments               = {SOURCEFILE} {MAXEVENTS} {DATASET} {DATASETGROUP} {SELECTION} {PERIOD} {JOBID}
Transfer_Input_Files    = {JOBNAME}/{EXECUTABLE},{JOBNAME}/{SOURCEFILE}
Output                  = {LOGNAME}/{JOBNAME}_$(Cluster)_$(Process).stdout
Error                   = {LOGNAME}/{JOBNAME}_$(Cluster)_$(Process).stderr
Log                     = {LOGNAME}/{JOBNAME}_$(Cluster)_$(Process).out
Requirements            = (OpSys == "LINUX") && (Arch != "DUMMY")
request_disk            = 1000000
request_memory          = 1600
#notify_user             = NOBODY@FNAL.GOV
use_x509userproxy       = TRUE
x509userproxy           = $ENV(X509_USER_PROXY)
should_transfer_files   = YES
when_to_transfer_output = ON_EXIT
Queue 1
'''.format(**self.config)

        #print writeme
        with open(self.jdl_name, 'w') as f:
            f.write(writeme)
        return

    def append_jdl(self):
        writeme = \
'''Arguments               = {SOURCEFILE} {MAXEVENTS} {DATASET} {DATASETGROUP} {SELECTION} {PERIOD} {JOBID}
Queue 1
'''.format(**self.config)

        #print writeme
        with open(self.jdl_name, 'a') as f:
            f.write(writeme)
        return

    def write_exe(self):

        writeme = \
'''#!/bin/bash

echo "Job submitted on host `hostname` on `date`"
echo "System `uname -a`"
echo ">>> arguments: $@"
#echo ">>> environment variables:"; printenv

MACHINE={MACHINE}
echo ">>> MACHINE=$MACHINE"

if [ $MACHINE == "ufl.edu" ] && [ ! -z $TMPDIR ]; then
    REMOTE_INITIAL_DIR=$(pwd)
    cp -pr * $TMPDIR
    cd $TMPDIR
    echo "*** On ufl.edu machine, cd from $REMOTE_INITIAL_DIR to $TMPDIR"
fi

RUNTIME_AREA=`pwd`
echo ">>> RUNTIME_AREA=$RUNTIME_AREA"

JOBID=$7
echo ">>> JOBID=$JOBID"

echo ">>> X509_USER_PROXY=$X509_USER_PROXY"

export SCRAM_ARCH={SCRAM_ARCH}
export CMSSW_VERSION={CMSSW_VERSION}
echo ">>> SCRAM_ARCH=$SCRAM_ARCH CMSSW_VERSION=$CMSSW_VERSION"

# Source cmsset_default.sh
if [ -f /cvmfs/cms.cern.ch/cmsset_default.sh ]; then
    export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
    . $VO_CMS_SW_DIR/cmsset_default.sh
    EXIT_STATUS=$?
elif [ -f "$VO_CMS_SW_DIR"/cmsset_default.sh ]; then
    . $VO_CMS_SW_DIR/cmsset_default.sh
    EXIT_STATUS=$?
elif [ -f "$OSG_APP"/cmssoft/cms/cmsset_default.sh ]; then
    . $OSG_APP/cmssoft/cms/cmsset_default.sh
    EXIT_STATUS=$?
fi
if [ $EXIT_STATUS -ne 0 ]; then echo "cmsset_default.sh exited with status=$EXIT_STATUS"; exit $EXIT_STATUS; fi

# Customize for this machine
if [ $MACHINE == "fnal.gov" ]; then
    echo ""
elif [ $MACHINE == "ufl.edu" ]; then
    echo ""
fi

# Setup CMSSW environment
scramv1 project CMSSW $CMSSW_VERSION
cd $CMSSW_VERSION
eval `scramv1 runtime -sh`
EXIT_STATUS=$?
if [ $EXIT_STATUS -ne 0 ]; then echo "scram runtime exited with status=$EXIT_STATUS"; exit $EXIT_STATUS; fi

SOFTWARE_DIR=`pwd`
echo ">>> SOFTWARE_DIR=$SOFTWARE_DIR"

# Download tarball
lcg-cp -v -b -D srmv2 --connect-timeout 180 "{TARBALL2}" ../{TARBALL}
EXIT_STATUS=$?
if [ $EXIT_STATUS -ne 0 ]; then echo "lcg-cp tarball exited with status=$EXIT_STATUS"; exit $EXIT_STATUS; fi

# Extract tarball
tar xzf ../{TARBALL}
EXIT_STATUS=$?
if [ $EXIT_STATUS -ne 0 ]; then echo "unpack tarball exited with status=$EXIT_STATUS"; exit $EXIT_STATUS; fi

echo ">>> Listing SOFTWARE_DIR"
ls -Al $SOFTWARE_DIR

# Return to working directory
cd $RUNTIME_AREA

echo ">>> Listing RUNTIME_AREA"
ls -Al $RUNTIME_AREA

# Modify source file
cat <<EOF >> modify_source_file.py
def chunk(l,n,i):
    m=(len(l)+n-1)/n
    return l[(i-1)*m:i*m]
def read_lines(f):
    return tuple(open(f))
def write_lines(f,lines):
    with open(f, 'w') as w:
        w.write(''.join(lines))
n={NJOBS}
i=$JOBID
f='{SOURCEFILE}'
write_lines(f, chunk(read_lines(f),n,i))
EOF

python modify_source_file.py

echo ">>> cat {SOURCEFILE}:"
cat {SOURCEFILE}

#echo ">>> which {ANALYZER}:"
#which {ANALYZER}

# Modify pset
mv $SOFTWARE_DIR/PSet.* $RUNTIME_AREA
cat <<EOF >> {PSETFILE}

i=$JOBID
f='{SOURCEFILE}'
process.source.fileNames = open(f).read().splitlines()
process.maxEvents.input = {MAXEVENTS}
process.TFileService.fileName = process.TFileService.fileName._value.replace('.root', '_%i.root' % i)
EOF

# Run the analyzer
#echo ">>> {ANALYZER} $@"
#{ANALYZER} $@
echo ">>> cmsRun {PSETFILE}"
cmsRun {PSETFILE}
EXIT_STATUS=$?
if [ $EXIT_STATUS -ne 0 ]; then echo "analyzer exited with status=$EXIT_STATUS"; exit $EXIT_STATUS; fi

echo ">>> Listing RUNTIME_AREA"
ls -Al $RUNTIME_AREA

# Transfer output file
ROOTFILE=`ls *_*.root`
lcg-cp -v -b -D srmv2 --connect-timeout 180 "$ROOTFILE" "{STORAGE}/$ROOTFILE"
EXIT_STATUS=$?
if [ $EXIT_STATUS -ne 0 ]; then echo "lcg-cp rootfile exited with status=$EXIT_STATUS"; exit $EXIT_STATUS; fi

# Done
rm input.txt modify_source_file.py *.tgz *.root
echo "Exit status is $EXIT_STATUS"
echo "Job finished on host `hostname` on `date`"
'''.format(**self.config)

        #print writeme
        with open(self.exe_name, 'w') as f:
            f.write(writeme)
        os.chmod(self.exe_name, 0744)  # make executable
        return

    def upload_tarball(self):
        # Only upload once
        if not open(self.chk_name,'r').read() == '':
            return

        print "[INFO   ] Uploading tarball ..."
        commands = \
'''lcg-cp -v -b -D srmv2 --connect-timeout 180 {TARBALL} {TARBALL2} >& /dev/null
'''.format(**self.config)
        self.execute_commands(commands)
        return

    def add_check(self):
        commands = \
'''echo {JOBPATH} {NJOBS} `wc -l < {SOURCEFILE}` >> {CHECKFILE}
'''.format(**self.config)
        self.execute_commands(commands)
        return

    def submit_jobs(self):
        commands = \
'''mv {EXECUTABLE} {SOURCEFILE} {JOBAD} {JOBPATH}/{JOBNAME}/
cd {JOBPATH}/ && condor_submit {JOBNAME}/{JOBAD}
'''.format(**self.config)
        self.execute_commands(commands)
        return


# ______________________________________________________________________________
def main():

    if len(sys.argv) < 7:
        raise Exception('Expect 6 command line arguments, received %i' % (len(sys.argv)-1))

    # Check GRID proxy
    try:
        a = subprocess.check_output(['voms-proxy-info'])
    except subprocess.CalledProcessError:
        print('%sPlease make sure you have a valid GRID proxy!%s' % ('\033[91m', '\033[0m'))
        raise

    print('[INFO   ] Creating condor jobs ...')
    print('[INFO   ] Command line arguments: %s' % (' '.join(sys.argv[1:])))

    job = CondorJobType()
    job.safety_check()

    print('[INFO   ] Job directory: %s' % job.config['JOBPATH'])

    job.run()

    print('[INFO   ] %s%i jobs are submitted to condor.%s' % ('\033[92m', job.config['NJOBS'], '\033[0m'))


# ______________________________________________________________________________
if __name__ == '__main__':

    main()
