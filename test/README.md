# photic/test

This folder contains Photic's unit tests. These can be built as a Makefile
project with `make test` and then run with `./TestMain`. Most (but not all)
of these tests have no STL dependencies and so can run on actual flight
hardware (see `TestMain.cpp` for details on which tests can run).