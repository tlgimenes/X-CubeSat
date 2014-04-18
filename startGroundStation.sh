#! /bin/bash
#
# demarrerStationSol.sh
# Copyright (C) 2014 tlgimenes <tlgimenes@gmail.com>
#
# Distributed under terms of the MIT license.
#

echo "Starting Ground Station of Ecole Polytechnique"

echo "Starting Gpredict"
../gpredict/src/gpredict  &
echo "Starting X-CubeSat Controler"
sleep 2
./XCubeSat_Controler 
