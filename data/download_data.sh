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
        mkdir porphyrin
        cd porphyrin
        wget -c https://heibox.uni-heidelberg.de/f/4a8546aad68f47579297/?dl=1 -O xaa
        wget -c https://heibox.uni-heidelberg.de/f/0d9ad8ceb52a4a1eb348/?dl=1 -O xab
        wget -c https://heibox.uni-heidelberg.de/f/9b87284e54d84817820f/?dl=1 -O xac
        wget -c https://heibox.uni-heidelberg.de/f/06a5209b09e3430ebeb5/?dl=1 -O xad
        wget -c https://heibox.uni-heidelberg.de/f/3fb3a9bde0124476a173/?dl=1 -O xae
        wget -c https://heibox.uni-heidelberg.de/f/76ffad119624446a82be/?dl=1 -O xaf
        wget -c https://heibox.uni-heidelberg.de/f/a7b65ae75cdc48b5820e/?dl=1 -O xag
        wget -c https://heibox.uni-heidelberg.de/f/77e59d12eb26476e8f05/?dl=1 -O xah
        cat xa* > porphyrin.zip
        rm xaa xab xac xad xae xaf xag xah
    ;;
    q ) exit 0
    ;;
    * ) echo "This option is not available. Please choose another one."
    ;;

esac
