#!/bin/bash

if [ -z "$1" ] ; then 
    echo "Incorrect usage of $0."
    echo "Enter the file that contains the directories that you want to graph as the first command line argument"
    exit
fi

echo "Choose a parameter to graph"
select param in "Crossover" "Mutation" "Quit"; do
    if [ "$param" = "Quit" ]; then 
        exit
    elif [ "$param" = "Mutation" ] || [ "$param" = "Crossover" ]; then
        break;
    fi
done

echo "Choose a fitness to graph"
select opt in "Average" "Minimum" "Both" "Quit"; do
    if [ "$opt" = "Quit" ]; then 
        exit
    elif [ "$opt" = "Average" ]; then
       opt="Aver"
       break
    elif [ "$opt" = "Minimum" ]; then 
       opt="Min"
       break
    elif [ "$opt" = "Both" ]; then
        echo "Not implemented yet! :D"
    fi
done

rm -rf .toGraph
mkdir .toGraph

# format the data from the GA's output to gnuplot's input
for dir in $(cat "$1") ; do
    for DFG in $(ls "$dir") ; do
        if [ "$DFG" = "test_suite.sh" ]; then
            continue
        fi
    
        : > .toGraph/${DFG}_$(basename $dir).dat
        
        if [ -d "$dir/$DFG/output" ]; then
            results_path="output/"
        fi

        if [ "$param" = "Crossover" ] ; then
            for cross in 0 0.15 0.3 0.45 0.60 0.75 0.90 1 ; do
                tac $dir/$DFG/${results_path}cross_$cross.dat | grep -m 1 "$opt" | \
                awk -v cross="$cross" '{printf "%f\t%f\n", cross, $3 ;}' >> .toGraph/${DFG}_$(basename $dir).dat
            done
        
        elif [ "$param" = "Mutation" ]; then
            for mut in 0.1 0.075 0.05 0.025 0.01 0.0075 0.005 0.0025 0.001 0 ; do
                tac $dir/$DFG/${results_path}mut_$mut.dat | grep -m 1 "$opt" | \
                awk -v mut="$mut" '{printf "%f\t%f\n", mut, $3 ;}' >> .toGraph/${DFG}_$(basename $dir).dat
            done
        fi
    done
done

# find the average
for DFG in $(ls "$dir") ; do
    if [ "$DFG" = "test_suite.sh" ]; then
        continue
    fi

    num_files=$(cd .toGraph; find -iname "$DFG*" | wc -l)
    
    paste -d" " .toGraph/${DFG}* | nawk -v n="$num_files" '{
        for(i=0;i <= n-1;i++)
        {
            t1 = 2 + (i*2)
            temp1 = temp1 + $t1
        }
        
        print $1" "temp1/n
        
        temp1=0
    }' > .toGraph/$DFG.avg
    
    rm .toGraph/${DFG}*.dat
done

mkdir eps
mkdir pictures


# FIX!!!
gnuplot << INSTRUCTIONS
    filenames = "B2_25.avg B1_50.avg B2_100.avg B1_150.avg"
    ideal_25 = 480
    ideal_50 = 595
    ideal_100 = 936
    ideal_150 = 1795

    reset
    set terminal postscript eps color enhanced 
    set output "eps/$(basename -s .txt $1)_$opt.eps"
    
    set style data linespoints
    #set size 2, 2
    #set origin 0.0, 0

    set title "$param Rate vs Fitness ($(basename -s .txt $1))"
    set xlabel "$param Rate"
    set ylabel "Fitness"
    set grid
    
    plot for [data in filenames] ".toGraph/".data using 1:2 title data, ideal_25, ideal_50, ideal_100, ideal_150

    reset
    set terminal png enhanced
    set output "pictures/$(basename -s .txt $1)_$opt.png
    set style data linespoints
    #set size 2, 2
    #set origin 0.0, 0

    set title "$param Rate vs Fitness ($(basename -s .txt $1))"
    set xlabel "$param Rate"
    set ylabel "Fitness"
    set grid
    
    plot for [data in filenames] ".toGraph/".data using 1:2 title data, ideal_25, ideal_50, ideal_100, ideal_150
            
INSTRUCTIONS
