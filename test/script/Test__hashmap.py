# coding=utf-8

import sys
import os
import subprocess
import random
import re


with open("../cjstat/_hashmap.h", "r") as h:
    a = h.read()
    b = re.findall("#define HASHMAP_SIZE (\d+)", a)
    try:
        HASHMAP_SIZE = int(b[0])
        print "HASHMAP_SIZE=", HASHMAP_SIZE
    except IndexError:
        print "macro HASHMAP_SIZE not found in source code\n"
        exit(1)

TEST_COUNT = HASHMAP_SIZE * 3 # operation counts

log = open("tmp/Test__hashmap.log", "w")

def test(inputs, process, check_callback):
    inputs = inputs + "\n"
    log.write(inputs)
    process.stdin.write(inputs)
    reading = process.stdout.readline().strip()
    assert check_callback(reading)

try:
    prc = subprocess.Popen(
            "tmp/Test__hashmap.out", 
            shell=True, 
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE)

    beat = {}

    def get_a_random_command():
        cmd_list = "PPPPPPPPPPGGGGGGGGGGCCCCCI"  # different probabilities
        return random.choice(cmd_list)

    def get_a_random_string():
        return ''.join([chr(random.randint(ord('a'), ord('z'))) for _ in xrange(40)])
    
    for i in xrange(TEST_COUNT, 0, -1):
        if int(10.0*i/TEST_COUNT) != int(10.0*(i+1)/TEST_COUNT):
            print i,
        
        cmd = get_a_random_command()
        if cmd == "P":
            if len(beat) == HASHMAP_SIZE:
                continue
            key = get_a_random_string()
            value = get_a_random_string()
            if key not in beat:
                beat[key] = value
                test("P " + key + " " + value, prc, lambda r: r == "done")

        elif cmd == "G":
            if random.randint(1, 2) == 1 and len(beat) > 0:
                key = random.choice(beat.keys())  # simulate an existed key
                test("G " + key, prc, lambda r: r == beat[key])
            else:
                key = get_a_random_string()  # simulate a non existed key
                test("G " + key, prc, lambda r: r == "null")

        elif cmd == "C":
            if random.randint(1, 2) == 1 and len(beat) > 0:
                key = random.choice(beat.keys())  # simulate an existed key
                test("C " + key, prc, lambda r: r == "1")
            else:
                key = get_a_random_string()  # simulate a non existed key
                test("C " + key, prc, lambda r: r == "0")
        elif cmd == "I":
            test("I", prc, lambda r: r == ("1" if len(beat) == 0 else "0"))

    print 

    prc.stdin.write("Q\n")
    prc.wait()

except AssertionError, e:
    print "| ERROR"
    print "| " + e.message
    print "|"
    exit(1)

exit(0)

