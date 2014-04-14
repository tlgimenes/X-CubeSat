#! /bin/bash
#
# demarrerStationSol.sh
# Copyright (C) 2014 tlgimenes <tlgimenes@gmail.com>
#
# Distributed under terms of the MIT license.
#

echo "Starting Ground Station of Ecole Polytechnique"

../gpredict/src/gpredict  &
./XCubeSat_Controler 
