#!/usr/bin/env python
# coding=utf-8

import subprocess
import os

SAMPLE_PID = 2956

HSPERFDATA_PATH = "resource/hsperfdata_sample/%d" % (SAMPLE_PID)
JSTAT_OUT_PATH = "resource/hsperfdata_sample/%d.jstat_out" % (SAMPLE_PID)

def process(lines):
    ret = {}
    for line in lines:
        try:
            key, value = line.split('=')
            ret[key] = value
        except ValueError:
            pass
    return ret


def get_jstat_output():
    with open(JSTAT_OUT_PATH, "r") as f:
        return f.read().split('\n')

def get_hsperfdata_parser_output():
    outputs = subprocess.check_output(
            ["tmp/Test_parser.out", "resource/hsperfdata_sample/2956"])
    return outputs.split('\n')

js = process(get_jstat_output())
hs = process(get_hsperfdata_parser_output())

if js == hs:
    print "OK"
else:
    for key, value in js.items():
        if key not in hs:
            print "key", key, "existed in jstat output, but hsperfdata_parser not"
            print
        elif hs[key] != js[key]:
            print "key =",key
            print "hs[key] =", hs[key], "while", "js[key] = ", js[key]
    for key, value in hs.items():
        if key not in js:
            print "key", key, "existed in hsperfdata_parser, but jstat output not"
            print

print 
