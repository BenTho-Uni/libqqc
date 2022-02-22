#!/bin/bash

# This script will allow you to download prepared data for testing purpuses
# As these files are human readable, they are large, so be aware

echo "This script allows you to download larger prepared data sets for testing
purposes."
echo "These filles will be large, so be aware."
echo ""
echo "Which of the available data sets would you like to download?"
echo "1: H2O with 6-31G/Becke Grid, exported from Q-Chem (1.7 MB, zip)"
echo "2: H3COH with 6-31G/Becke Grid, exported from Q-Chem (15.3 MB, zip)"
echo "3: Anthracene with 6-31G/Becke Grid, exported from Q-Chem (3.6 GB, zip)"
echo "4: Porphyrine with 6-31G/Becke Grid, exported from Q-Chem (19.6 GB, zip)"
echo "q: Exit"

read choice

case $choice in 
    1 ) echo "Downloading H20 6-31G/Becke Grid"
        wget -c https://heibox.uni-heidelberg.de/f/ac3b070ce1d7440eb5bb/?dl=1 -O h2o.zip
    ;;
    2 ) echo "Downloading H3COH 6-31G/Becke Grid"
        wget -c https://heibox.uni-heidelberg.de/f/c357ad75f7904dc7af03/?dl=1 -O h3coh.zip
    ;;
    3 ) echo "Downloading Anthracene 6-31G/Becke Grid"
        wget -c https://heibox.uni-heidelberg.de/f/6979bfd3edfd4e1780bd/?dl=1 -O anthracene.zip
    ;;
    4 ) echo "Downloading Porphyrin 6-31G/Becke Grid in parts. This may take a while."
        wget -c https://heibox.uni-heidelberg.de/f/8f82321355004a17872d/?dl=1 -O porphyrin.z01
        wget -c https://heibox.uni-heidelberg.de/f/b4f9e0e3774040db8de9/?dl=1 -O porphyrin.z02
        wget -c https://heibox.uni-heidelberg.de/f/77afcb7c6c854f5f870b/?dl=1 -O porphyrin.z03
        wget -c https://heibox.uni-heidelberg.de/f/d3e8d60f8e1b473995df/?dl=1 -O porphyrin.z04
        wget -c https://heibox.uni-heidelberg.de/f/48411f4f86ed4966bb75/?dl=1 -O porphyrin.z05
        wget -c https://heibox.uni-heidelberg.de/f/09aa895cb7cc488b8a79/?dl=1 -O porphyrin.z06
        wget -c https://heibox.uni-heidelberg.de/f/4f84bf8fd19f480795e6/?dl=1 -O porphyrin.z07
        wget -c https://heibox.uni-heidelberg.de/f/5c8fcfb6a0da4f1aa8be/?dl=1 -O porphyrin.z08
        wget -c https://heibox.uni-heidelberg.de/f/1d398c2145fb4031ab86/?dl=1 -o porphyrin.zip
    ;;
    q ) exit 0
    ;;
    * ) echo "This option is not available. Please choose another one."
    ;;

esac
