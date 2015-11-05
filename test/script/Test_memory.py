#!/usr/bin/env python
# coding=utf-8

import subprocess
import os
import time

with open("resource/java_bomb/run_command", "r") as f:
    java_bomb_command = f.read().strip('\n')
    java_bomb_command = java_bomb_command.split(' ')

print "Run a jvm"
jvm = subprocess.Popen(java_bomb_command, stdout=subprocess.PIPE)
time.sleep(5)
print "pid=", jvm.pid

os.system("valgrind --tool=memcheck --leak-check=yes tmp/Test_memory.out " + str(jvm.pid) + " 1>/dev/null")

print "Kill the jvm"
jvm.kill()

