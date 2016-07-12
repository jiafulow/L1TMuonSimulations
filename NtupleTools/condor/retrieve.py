#!/usr/bin/env python

import glob
import os
import re
import sys
import subprocess

def get_dataset_name(jobpath):
    return jobpath.split('/')[-1]

class Retriever(object):

    def __init__(self, chk='.checkfile.txt', logname='res', outname='out', force=False, do_hadd=False):

        self.chk = chk
        self.logname = logname
        self.outname = outname
        self.force = force
        self.do_hadd = do_hadd
        self.datasets = []
        self.datasetgroups = {}

    def run(self):

        self.setup()
        if self.do_hadd:
            self.hadd()

    def setup(self):

        if not os.path.exists(self.chk):
            raise Exception('Cannot find check file %s?!' % self.chk)

        # Prepare datasets and datasetgroups
        with open(self.chk) as f:
            for line in f.readlines():
                jobpath, datasetgroup, selection, period, njobs, nfiles = line.rstrip('\n').split(' ')
                njobs = int(njobs)
                nfiles = int(nfiles)
                print('[INFO   ] Found jobpath %s with njobs %i' % (jobpath, njobs))

                self.datasets.append((jobpath, datasetgroup, selection, period, njobs, nfiles))

                if datasetgroup not in self.datasetgroups:
                    self.datasetgroups[datasetgroup] = []
                self.datasetgroups[datasetgroup].append(jobpath)

        # Check if jobs return successfully
        if not self.force:
            for dataset in self.datasets:
                jobpath = dataset[0]
                njobs = dataset[4]
                nfiles = dataset[5]

                logdir = os.path.join(jobpath, self.logname)
                out = subprocess.check_output('cd %s && grep "Exit status is 0" *.stdout | wc -l' % (logdir), shell=True)
                out = int(out)
                if out != njobs:
                    raise Exception('Expect %s %i jobs be completed, found %i' % (get_dataset_name(jobpath), njobs, out))

                out = subprocess.check_output('cd %s && grep "Successfully opened file .*\.root" *.stderr | wc -l' % (logdir), shell=True)
                out = int(out)
                if out != nfiles:
                    raise Exception('Expect %s %i input files be processed, found %i' % (get_dataset_name(jobpath), nfiles, out))
        return

    def hadd(self):

        for datasetgroup, jobpaths in sorted(self.datasetgroups.iteritems()):
            print('[INFO   ] Found datasetgroup %s with datasets [%s]' % (datasetgroup, ','.join([get_dataset_name(x) for x in jobpaths])))

            files = []
            files_glob = []
            for jobpath in jobpaths:
                files.append('%s/%s/*.root' % (jobpath, self.outname))
                files_glob += glob.glob(files[-1])

            if not self.force:
                if not files or not files_glob:
                    continue

            if not files or not files_glob:
                raise Exception('No files to be added.')

            # Decide target file name using the first source file name
            m = re.match('(.*?)_.*\.root', os.path.basename(files_glob[0]))
            if not m:
                raise Exception('Cannot parse source file name such as this: %s' % (os.path.basename(files_glob[0])))

            target = "%s_%s.root" % (m.group(1), datasetgroup)

            print('[INFO   ] Calling hadd')
            print('  hadd -f %s %s' % (target, ' '.join(files)))
            subprocess.call('hadd -f %s %s' % (target, ' '.join(files)), shell=True)
            print('[INFO   ] %s%s is created (%iM).%s' % ('\033[92m', target, os.stat(target).st_size >> 20, '\033[0m'))
        return


# ______________________________________________________________________________
def main():

    print('[INFO   ] Retrieving condor jobs ...')

    force = False
    hadd = False

    for arg in sys.argv[1:]:
        if arg == "--force" :
            force = True  # do not raise exception
        if arg == "--hadd":
            hadd = True  # do hadd

    ret = Retriever(force=force, do_hadd=hadd)
    ret.run()


# ______________________________________________________________________________
if __name__ == '__main__':

    main()
