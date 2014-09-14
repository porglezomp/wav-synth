#!/usr/bin/python

import math
import re
import sys
import os

infile = "score.pyscore"
outfile = "score.cscore"

if len(sys.argv) > 1:
    if os.path.isfile(sys.argv[1]):
        infile = sys.argv[1]
        if ".pyscore" in infile:
            outfile = infile.replace(".pyscore", ".cscore")
        else:
            outfile = infile + ".out"
    else:
        print("File '%s' does not exist." % sys.argv[1])
        exit(1)
if len(sys.argv) > 2:
    outfile = sys.argv[2]

# Frequencies for octave 8
basis = [4186.01, 4434.92, 4698.63, 4978.03, 
         5274.04, 5587.65, 5919.91, 6271.93,
         6644.88, 7040.00, 7458.62, 7902.13]
notes_map = {"c": 0, "d": 2, "e": 4, "f": 5, "g": 7, "a": 9, "b": 11}

# Generate a frequency based on a note description
def frequency(note):
    base = 0
    octave = 0
    accidental = 0
    if len(note) > 0:
        base = notes_map[note[0]]
    if len(note) > 1:
        octave = int(note[1])
    if len(note) > 2:
        if note[2] == "b":
            if len(note) > 3 and note[3] == "b":
                accidental = -2
            else:
                accidental = -1
        elif note[2] == "#":
            if len(note) > 3 and note[3] == "#":
                accidental = 2
            else: 
                accidental = 1
        elif note[2] == "x":
            accidental = 2

    n = base+accidental
    if n > 11 or n < 0:
        octave += math.floor(n/12)
        n %= 12
    exponent = 8-octave
    return basis[n] / 2**exponent

getsbeat = 4
beats = 4
measure_length = 0
beat_length = 0
quarter_length = 0
measure = 0

def meter(m):
    global beats, getsbeat
    m = m.split("/")
    beats = int(m[0].strip())
    getsbeat = int(m[1].strip())
    print("%i/%i" % (beats, getsbeat))

def tempo(t):
    global beat_length, measure_length, quarter_length
    names = {"whole": 1, "half": 2, "quarter": 4}
    t = t.split("=")
    n = names[t[0].strip()]
    time = int(t[1].strip())
    seconds = 60.0/time # Seconds per n
    seconds *= n # Seconds per whole note
    beat_length = seconds/getsbeat
    measure_length = beat_length * beats
    quarter_length = beat_length * 4.0 / getsbeat

    print("a measure lasts %f seconds, a beat %f, a quarter %f" % 
            (measure_length, beat_length, quarter_length))

note_lengths = {'w': 4, 'h': 2, 'q': 1, 'e': 0.5, 's': 0.25}
def gen_start(s):
    time = 0
    for item in s.split("+"):
        n = re.search("[whqes]", item).group()
        # print(n)
        n = note_lengths[n]

        if "t" in item:
            # print(triplet.group())
            n *= 2/3.0;
        count = int(re.search("\d", item).group())
        # print(count)
        time += (count-1)*n*quarter_length
    if time > measure_length:
        print("error on line", line_no, " note starts after measure")
    time += (measure-1)*measure_length
    return time

handlers = ["meter", "tempo"]
handler_functions = {
    "meter": meter,
    "tempo": tempo
}
values = {}

notes = []
measure_notes = []

def finalize_measure():
    global measure_notes, notes
    measure_notes.sort()
    notes.extend(measure_notes)
    measure_notes = []

line_no = 0
end_time = 0
with open(infile, "r") as f:
    ###################
    # Read the header #
    ###################
    while True:
        line = f.readline()
        line_no += 1
        if len(line) == 0: exit(0) # EOF
        line = line.strip()
        if len(line) == 0 or line[0] == "#": continue # Line is a comment

        if ":" in line:
            pair = line.split(":")
            values[pair[0].strip()] = pair[1].strip()

        if "---" in line:
            measure += 1
            break

    ##############################
    # Process header information #
    ##############################
    for handler in handlers:
        # Is there a value that this handler can process?
        if handler in values:
            handler_functions[handler](values[handler])

    ####################
    # Process the body #
    ####################
    while True:
        line_no += 1
        line = f.readline().lower()
        if line == "":
            finalize_measure()
            break # EOF
        line = line.strip()
        if len(line) == 0 or line[0] == "#": continue # Comment

        if "---" in line: # Measure break
            finalize_measure()
            measure += 1
            continue

        line = re.split(r'\s+', line, 1)

        start = line[0]
        start_time = gen_start(start)
        tokens = line[1]
        tokens = tokens.split(",")
        for token in tokens:
            elements = re.split("\s+", token.strip())
            pitch = frequency(elements[0])
            # print(elements[0], pitch)
            base_note = re.search("[whqes]", elements[1]).group()
            base_len = note_lengths[base_note]
            if re.search("t", elements[1]):
                base_len *= 2/3.0
            num_dots = len(re.findall("\.", elements[1]))
            note_len = sum([base_len*0.5**x for x in range(num_dots+1)])
            note_len *= quarter_length
            # print(elements[1], base_len, note_len)
            measure_notes.append((start_time, note_len, pitch))
            # print(start_time)
            if start_time + note_len > end_time:
                end_time = start_time + note_len

end_padding = 1.0
print("%.3f seconds long" % end_time)
with open(outfile, "w") as f:
    f.write("%i notes\n" % len(notes))
    f.write("%.3f seconds\n\n" % (end_time + end_padding))
    for note in notes:
        f.write("%.2f %.3f %.3f\n" % (note[2], note[0], note[1]))