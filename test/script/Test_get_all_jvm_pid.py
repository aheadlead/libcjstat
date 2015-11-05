# coding=utf-8

import sys
import os
import subprocess

JAVA = sys.argv[1]
JPS = sys.argv[2]

JAVA_PROCESS_NUM = 5

hd = [None] * JAVA_PROCESS_NUM
pids = []

exit_code = 0

try:
    print "Start %d java processes..." % JAVA_PROCESS_NUM
    for i in range(JAVA_PROCESS_NUM):
        hd[i] = subprocess.Popen([JAVA, "-cp", "tmp", "bomb"])
        pids.append(hd[i].pid)
        print hd[i].pid

    print "Read java process info from jps..."
    outputs = subprocess.check_output(JPS + " | awk '{ print $1 }'", shell=True)
    pids_jps = set([int(_) 
        for _ in filter(lambda x: x != '', outputs.split('\n'))])
    print pids_jps

    print "Read java process info from hsperfdata_parser/util.h"
    outputs = subprocess.check_output("tmp/Test_get_all_jvm_pid.out", shell=True)
    outputs = outputs.strip().split('\n')
    outputs = [_.split(' ') for _ in outputs]
    pidl = [_[0] for _ in outputs]
    pathl = [_[1] for _ in outputs]
    pids_hsp = set([int(_)
        for _ in filter(lambda x: x != '', pidl)])
    print pids_hsp

    # pids_jps included jps process
    assert pids_hsp < pids_jps, \
            "Pid list from hsperfdata_parser is not valid."

    print "Check if the files exists"
    for path in pathl:
        print path,
        assert os.access(path, os.F_OK), \
                "File " + path + " is not found."
        print " OK"
except AssertionError, e:
    print "| ERROR"
    print "| " + e.message
    print "|"
    exit_code = 1
finally:
    print "Stop java processes..."
    for i in range(JAVA_PROCESS_NUM):
        print "kill %d" % hd[i].pid
        hd[i].kill()

exit(exit_code)

