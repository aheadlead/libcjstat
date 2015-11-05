#!/usr/bin/env python
# coding=utf-8

import subprocess
import os
import time
import signal

with open("resource/java_bomb/run_command", "r") as f:
    java_bomb_command = f.read().strip('\n')
    java_bomb_command = java_bomb_command.split(' ')

def process(lines):
    ret = {}
    for line in lines:
        try:
            key, value = line.split('=')
            if "perfdata" not in key:  # filter perfdata info
                ret[key] = value
        except ValueError:
            pass
    return ret


def get_jstat_output(pid):
    outputs = subprocess.check_output(
            ["jstat", "-J-Djstat.showUnsupported=true", "-snap", str(pid)])
    return outputs.split('\n')

def get_hsperfdata_parser_output(pid):
    outputs = subprocess.check_output(
            ["tmp/Test_get_hsperfdata_by_pid_full.out", str(pid)])
    return outputs.split('\n')

print "Run a jvm"
jvm = subprocess.Popen(java_bomb_command, stdout=subprocess.PIPE)
time.sleep(5)

print "Pause the jvm"
jvm.send_signal(signal.SIGSTOP)

print "Read from jstat"
js = process(get_jstat_output(jvm.pid))

print "Read from hsperfdata_parser"
hs = process(get_hsperfdata_parser_output(jvm.pid))

print "Kill the jvm"
jvm.kill()

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

