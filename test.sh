#!/usr/bin/env bash

make clean
make all
./stacka programs/stack/lngjif.stack programs/stack/lngjif
time for run in {1..1000}; do ./stckvm programs/stack/lngjif inline; done