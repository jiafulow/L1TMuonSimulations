#!/usr/bin/env python

import glob
import os
import re
import sys
import subprocess

def get_dataset_name(jobpath):
    return jobpath.split('/')[-1]

class Retriever(object):

    def __init__(self, checkfile='.checkfile.txt', jobname='job/', logname='res/', outname='out/', do_hadd=False):
        self.checkfile = checkfile
        self.jobname = jobname
        self.logname = logname
        self.outname = outname
        self.do_hadd = do_hadd
        self.datasets = {}
        self.datasetgroups = {}

    def run(self):
        self.report()
        if self.do_hadd:
            self.hadd()

    def report(self):
        if not os.path.exists(self.checkfile):
            raise Exception('Cannot find check file %s?!' % self.checkfile)

        # Prepare datasets and datasetgroups
        ds_list = []
        with open(self.checkfile) as f:
            for line in f:
                if line == '' or line.lstrip().startswith('#'):
                    continue
                jobpath, datasetgroup, selection, period, njobs, nfiles = line.rstrip('\n').split(' ')
                njobs = int(njobs)
                nfiles = int(nfiles)
                ds = get_dataset_name(jobpath)
                dsg = datasetgroup
                self.datasets[ds] = (jobpath, datasetgroup, selection, period, njobs, nfiles)
                if dsg not in self.datasetgroups:
                    self.datasetgroups[dsg] = []
                self.datasetgroups[dsg].append(ds)
                ds_list.append(ds)

        # Check stdout and stderr
        print('[INFO   ] Found %i datasets, %i datasetgroups' % (len(ds_list), len(self.datasetgroups)))
        for ds in ds_list:
            print('          .. %s' % (ds))

        print('+--------------------+--------------------------------------+---------+--------+---------+')
        print('| %-18s | %-36s | %-7s | %-6s | %-7s |' % ('datasetgroup', 'dataset', 'njobs', '%jobs', 'nfiles'))
        print('+--------------------+--------------------------------------+---------+--------+---------+')

        goodsites, badsites = {}, {}

        for dsg, v in sorted(self.datasetgroups.iteritems()):
            for ds in v:
                jobpath, datasetgroup, selection, period, njobs, nfiles = self.datasets[ds]
                ngoodjobs, ngoodfiles = 0, 0

                logdir = os.path.join(jobpath, self.logname)
                logs = glob.glob(logdir+'*')

                prefix = ''
                if logs:
                    get_prefix = lambda x: x[:x.rfind('_')]
                    prefix = get_prefix(logs[0])

                msg = ''
                if prefix:
                    for i in xrange(njobs):
                        # Look for *.out, *.stdout, *.stderr
                        fname = prefix + '_%i.out' % i
                        if not os.path.isfile(fname):
                            msg += ('          .. Cannot find %s\n' % fname)
                            continue
                        fname = prefix + '_%i.stdout' % i
                        if not os.path.isfile(fname):
                            msg += ('          .. Cannot find %s\n' % fname)
                            continue
                        fname = prefix + '_%i.stderr' % i
                        if not os.path.isfile(fname):
                            msg += ('          .. Cannot find %s\n' % fname)
                            continue

                        # Get exit status
                        fname = prefix + '_%i.stdout' % i
                        with open(fname) as f:
                            text = f.read()
                            # Which site?
                            m = re.search('GLIDEIN_CMSSite=(\w+)', text)
                            if m:
                                site = m.group(1)
                            else:
                                site = 'unknown'

                            # Successful?
                            m = re.search('Job finished on ', text)
                            if m:
                                ngoodjobs += 1  # good job!
                                goodsites[site] = goodsites.get(site, 0) + 1

                                m = re.findall('Successfully opened file ', text)
                                ngoodfiles += len(m)
                                continue
                            # Failed?
                            badsites[site] = badsites.get(site, 0) + 1
                            m = re.search('status=(\d+)', text)
                            if m:
                                status = int(m.group(1))
                                msg += ('          .. %s, %s, has exit status=%i\n' % (fname, site, status))
                                continue
                            else:
                                msg += ('          .. %s, %s, has exit status=unknown\n' % (fname, site))
                                continue

                # Print summary
                s_pjobs = '%.1f%%' % (100.*ngoodjobs/njobs)
                if ngoodjobs == njobs:
                    s_pjobs = '%s%s%s' % ('\033[92m', s_pjobs, '\033[0m')
                s_njobs = '%i/%i' % (ngoodjobs, njobs)
                s_nfiles = '%i/%i' % (ngoodfiles, nfiles)

                print('| %-18s | %-36s | %7s | %6s | %7s |' % (dsg, ds, s_njobs, s_pjobs, s_nfiles))
                if msg:
                    msg = ('          Log files are in %s\n' % logdir) + msg
                    print('%s%s%s' % ('\033[37m', msg, '\033[0m'))

        print('+--------------------+--------------------------------------+---------+--------+---------+')

        s_goodsites = ', '.join(['%s (%i)' % (k,v) for k,v in sorted(goodsites.iteritems(), key=lambda x: x[1], reverse=True)])
        s_badsites = ', '.join(['%s (%i)' % (k,v) for k,v in sorted(badsites.iteritems(), key=lambda x: x[1], reverse=True)])
        print('[INFO   ] Good sites: %s' % s_goodsites)
        print('[INFO   ] Bad sites: %s' % s_badsites)
        return

    def hadd(self):
        for dsg, v in sorted(self.datasetgroups.iteritems()):
            print('[INFO   ] Get datasetgroup %s: %s' % (dsg, repr(v)))

            jobpaths = []
            for ds in v:
                jobpath, datasetgroup, selection, period, njobs, nfiles = self.datasets[ds]
                jobpaths.append(jobpath)

            files = []
            files_glob = []
            for jobpath in jobpaths:
                files.append('%s/%s' % (jobpath, self.outname+'*.root'))
                files_glob += glob.glob(files[-1])
            if not files or not files_glob:
                raise Exception('No files to be added.')

            # Decide target file name using the first filename
            m = re.match('(.*?)_.*\.root', os.path.basename(files_glob[0]))
            if m:
                target = '%s_%s.root' % (m.group(1), dsg)
            else:
                raise Exception('Cannot parse filename such as this: %s' % (os.path.basename(files_glob[0])))

            cmd = '          hadd -f %s %s' % (target, ' '.join(files))

            print('[INFO   ] Calling hadd')
            print('%s%s%s' % ('\033[37m', cmd, '\033[0m'))
            subprocess.check_call(cmd.strip(), shell=True)
            print('[INFO   ] %s%s is created (%iM).%s' % ('\033[92m', target, os.stat(target).st_size >> 20, '\033[0m'))
        return


# ______________________________________________________________________________
def main():

    print('[INFO   ] Retrieving condor jobs ...')

    # Parse input arguments
    do_hadd = False
    for arg in sys.argv[1:]:
        if arg == '--hadd':
            do_hadd = True  # do hadd

    ret = Retriever(do_hadd=do_hadd)
    ret.run()

# ______________________________________________________________________________
if __name__ == '__main__':

    main()
