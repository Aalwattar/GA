#!/bin/bash

# run this file with the directory large_pop_tests
CROSS_RATE=0.90

if [ -z "$1" ] ; then 
    echo "Incorrect usage of $0."
    echo "Enter the file that contains the directories that you want to graph as the first command line argument"
    exit
fi

rm -fr .toGraph
mkdir .toGraph

for dir in $(cat "$1") ; do
    BENCHMARKS=$(ls "$dir")
    
    for DFG in $BENCHMARKS ; do
        echo "# Generation_#  Average  Min" > .toGraph/${DFG}_$(basename $dir).dat
        cat $dir/$DFG/output/cross_$CROSS_RATE.dat | grep "Aver\|Min\|-----------" | \
        awk '{if(NR % 3 == 1) printf "%d\t", $3 ; 
                else if(NR % 3 == 2) printf "%f\t", $3 ;
                else print $3; }' >> .toGraph/${DFG}_$(basename $dir).dat
    done
done

mkdir eps
mkdir pictures

#Graph together
for filename in $BENCHMARKS ; do 
    rm -f .gnuplot_script
    touch .gnuplot_script
    
    DFG=$( find -iname "${filename}*" | grep ".toGraph" | awk '{printf "%s ", $0}' )
    
    echo "reset" >> .gnuplot_script
    echo "set terminal postscript eps enhanced" >> .gnuplot_script
    echo "set output \"eps/$filename.eps\"" >> .gnuplot_script
    
    echo 'set style data linespoints' >> .gnuplot_script

    echo 'set title "Generations vs Fitness"' >> .gnuplot_script
    echo 'set xlabel "Generation"'  >> .gnuplot_script
    echo 'set ylabel "Fitness"' >> .gnuplot_script
    echo 'set grid' >> .gnuplot_script
    
    echo "plot for [ file in \"$DFG\" ] file using 1:2 notitle" >> .gnuplot_script
#     echo "   file using 1:3 title Minimum"  >> .gnuplot_script 
            
    echo "reset" >> .gnuplot_script
    echo "set terminal png enhanced" >> .gnuplot_script
    echo "set output \"pictures/$filename.png\"" >> .gnuplot_script
    
    echo 'set style data linespoints' >> .gnuplot_script

    echo 'set title "Generations vs Fitness"' >> .gnuplot_script
    echo 'set xlabel "Generation"' >> .gnuplot_script
    echo 'set ylabel "Fitness"' >> .gnuplot_script
    echo 'set grid' >> .gnuplot_script
    
    echo "plot for [ file in \"$DFG\" ] file using 1:2 notitle" >> .gnuplot_script
#     echo "   file using 1:3 title Minimum"  >> .gnuplot_script 
    
    gnuplot .gnuplot_script

done

rm -rf .toGraph
rm .gnuplot_script