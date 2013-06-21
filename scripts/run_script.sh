#!/bin/bash

BUILD_DIR="build"
ARCH_FILE="${BUILD_DIR}/input/architecture_library.txt"

INPUTS="B2_25_40.aif B1_50_50.aif B2_100_120.aif B1_150_200.aif"
RUN_ARGS="./${BUILD_DIR}/OfflineScheduler.exe -a $ARCH_FILE"

if [ -z "$1" ] ; then 
    echo "Incorrect usage of $0."
    echo "This program requires that the first line of this program be the output directory name"
    exit
fi

output_dir=$1
mkdir output/$output_dir

if [ "$(basename $PWD)" = "scripts" ]; then 
    cd .. ;
fi

# set build
echo "What type of data do you wish to collect?"
select MAKE_ARGS in "Generational - ALL" "Generational - Summary Statistics" "Final Result Only" "Quit"; do
    if [ "$MAKE_ARGS" = "Quit" ]; then 
        exit
    elif [ "$MAKE_ARGS" = "Generational - ALL" ]; then
       cd $BUILD_DIR; make clean; make verbose; cd ..
       break
    elif [ "$MAKE_ARGS" = "Generational - Summary Statistics" ]; then 
       cd $BUILD_DIR; make clean; make exe; cd ..
       break
    elif [ "$MAKE_ARGS" = "Final Result Only" ]; then
       cd $BUILD_DIR; make clean; make final; cd ..
       break
    fi
done

# set fitness funciton
echo "Include Power in fitness calculations?"
select opt in "Yes" "No" "Quit"; do
    if [ "$opt" = "Quit" ]; then 
        exit
    elif [ "$opt" = "Yes" ]; then
        break
    elif [ "$opt" = "No" ]; then
        RUN_ARGS="$RUN_ARGS -w 1.0 "
        break
    fi
done


echo "Choose a parameter to graph"
select args in "Crossover" "Mutation" "Quit"; do
    if [ "$args" = "Quit" ]; then 
        exit
    elif [ "$args" = "Mutation" ] || [ "$args" = "Crossover" ]; then
        break
    fi
done


if [ "$args" = "Crossover" ]; then
    for input in $INPUTS ; do 
        DFG_DIR=$( echo $input | awk 'BEGIN {FS="_"}{printf "%s_%d", $1, $2}')
        mkdir "output/${output_dir}/${DFG_DIR}"
    
        $(for cross in 0.0 0.15 0.3 0.45 0.60 0.75 0.90 1.0 ; do
            (time $RUN_ARGS -d ${BUILD_DIR}/input/$input -c $cross > output/${output_dir}/${DFG_DIR}/cross_$cross.dat) 2>> output/${output_dir}/${DFG_DIR}/cross_$cross.dat
            sleep 1
        done ) &
    done
fi 


if [ "$args" = "Mutation" ]; then
    for input in $INPUTS ; do 
        DFG_DIR=$( echo $input | awk 'BEGIN {FS="_"}{printf "%s_%d", $1, $2}')
        mkdir "output/${output_dir}/${DFG_DIR}"
        
        $(for mut in 0.1 0.075 0.05 0.025 0.01 0.0075 0.005 0.0025 0.001 0.0 ; do
            (time $RUN_ARGS -d ${BUILD_DIR}/input/$input -m $mut > output/${output_dir}/${DFG_DIR}/mut_$mut.dat) 2>> output/${output_dir}/${DFG_DIR}/mut_$mut.dat
            sleep 1
        done) &
    done
fi

