wav-synth
=========

WAV Synth is a package for synthesizing music from a music format.

Overview
--------

With WAV synth, you write a musical score in a human readable music format,
and then run it through a converter which turns it into a much more basic format
for synthesis.
Sound.exe can take one of these compiled scores and turn it into a .WAV file for
your enjoyment.

If you want a high quality synth for music production, this is not for you,
but if you're looking for a small, simple, hackable synth to mess around with,
WAV Synth is a good choice.

Example Usage
-------------

Running `python convert.py mario.pyscore` will produce a compiled score file 
called `mario.cscore`, and then running `./Sound.exe cscore` will produce
a sound file called `out.wav`.

Getting Started
---------------

WAV Synth is a C program with no dependencies.
To compile it, you'll need a C compiler (tested on clang, gcc, and mingw).
If you're using mingw, run `mingw32-make` to run the makefile and compile it,
if you're using any other compiler, you should be able to just type `make`.

`converter.py` is a Python program, and so it naturally has Python as a dependency.
To convert a .pyscore to a .cscore, run `python converter.py <pyscore file> [<output cscore>]`.

To produce a wav file, run `./Sound.exe <input cscore> [<output WAV>]`

Credits
-------

Developed by Caleb Jones (@porglezomp).

License
-------

Licensed under the MIT license.
