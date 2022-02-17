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
        wget https://heibox.uni-heidelberg.de/f/ac3b070ce1d7440eb5bb/?dl=1 -O h2o.zip
    ;;
    2 ) echo "Downloading H3COH 6-31G/Becke Grid"
        wget https://heibox.uni-heidelberg.de/f/c357ad75f7904dc7af03/?dl=1 -O h3coh.zip
    ;;
    3 ) echo "DOwnloading Anthracene 6.31G/Becke Grid"
        wget https://heibox.uni-heidelberg.de/f/6979bfd3edfd4e1780bd/?dl=1 -O anthracene.zip
    ;;
    q ) exit 0
    ;;
    * ) echo "This option is not available. Please choose another one."
    ;;

esac
