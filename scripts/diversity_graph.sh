#!/bin/bash

####################################################################################################
# USAGE:
#   place the full names (path + directory name) of all directories that you want to plot on the 
#   same graph inside a file by using a combination of 
#
#   1) find -iname "XXXX*" where XXXX is the name of the directory, and * represents repitions of
#	that type of test (eg. mutation_tests_2, mutation_tests_3 ... etc.)
#   2) grep -v "STUFF-YOU-DON'T-WANT-IN-YOUR-DIRECTORY-NAMES" 
#
#   Through the usage of the above two commands + redirecting the output into a file you can create
#   a file that contains a list of all the directories that you wish to graph together. 
#
# RUNNING:
#   type "bash diversity_graph.sh <textfileName>"
#
####################################################################################################

#PLEASE DON'T CHANGE!
CROSS_RATE=0.90

if [ -z "$1" ] ; then 
    echo "Incorrect usage of $0."
    echo "Enter the file that contains the directories that you want to graph as the first command line argument"
    exit
fi

rm -fr .toGraph
mkdir .toGraph

# translate the GA's output into gnuplot's format
for dir in $(cat "$1") ; do
    BENCHMARKS=$(ls "$dir")
    
    for DFG in $BENCHMARKS ; do
        echo "# Generation_#  SD  sum_hamming hamming_hamming unique" > .toGraph/${DFG}_$(basename $dir).dat
        cat $dir/$DFG/cross_$CROSS_RATE.dat | grep "SD\|Hammig\|-----------" | \
        awk 'NR <= 1200 {if(NR % 3 == 2) printf "%d\t", $3 ; 
                else if(NR % 3 == 0) printf "%s\t%s\t%s\n", substr($4,2,8), substr($5,3,8), substr($9,4,8) ;
                else printf "%d\t", $3; }' >> .toGraph/${DFG}_$(basename $dir).dat

	#NOTE - 1200 = 400 generations * 3 lines of data per generation
    done
done

#create output directories
mkdir -p eps
mkdir -p eps/standard_deviation
mkdir -p eps/sum_haming
mkdir -p eps/haming_of_haming
mkdir -p eps/percentage_unique

mkdir -p pictures
mkdir -p pictures/standard_deviation
mkdir -p pictures/sum_haming
mkdir -p pictures/haming_of_haming
mkdir -p pictures/percentage_unique

#Graph together using gnuplot
for filename in $BENCHMARKS ; do 
    rm -f .gnuplot_script
    touch .gnuplot_script
    
    DFG=$( find -iname "${filename}*" | grep ".toGraph" | awk '{printf "%s ", $0}' )

    # make the eps files
    echo "reset" >> .gnuplot_script
    echo "set terminal postscript eps color enhanced" >> .gnuplot_script
    
    echo 'set style data linespoints' >> .gnuplot_script
    echo 'set grid' >> .gnuplot_script
    
    echo "set title \"Generations vs Standard Deviation ($filename) \"" >> .gnuplot_script
    echo 'set xlabel "Generation"'  >> .gnuplot_script
    echo 'set ylabel "Standard Deviation"' >> .gnuplot_script
echo "set output \"eps/standard_deviation/$filename.eps\"" >> .gnuplot_script
    echo "plot for [ file in \"$DFG\" ] file using 1:2 notitle" >> .gnuplot_script # standard Deviation

    echo "set title \"Generations vs Sum of Haming Distances ($filename) \"" >> .gnuplot_script
    echo 'set xlabel "Generation"'  >> .gnuplot_script
    echo 'set ylabel "Sum of Haming Distances"' >> .gnuplot_script
echo "set output \"eps/sum_haming/$filename.eps\"" >> .gnuplot_script
    echo "plot for [ file in \"$DFG\" ] file using 1:3 notitle" >> .gnuplot_script # sum of the hamming distances

    echo "set title \"Generations vs Haming Distance of Haming Distances ($filename) \"" >> .gnuplot_script
    echo 'set xlabel "Generation"'  >> .gnuplot_script
    echo 'set ylabel "Haming Distance of Haming Distances"' >> .gnuplot_script
echo "set output \"eps/haming_of_haming/$filename.eps\"" >> .gnuplot_script
    echo "plot for [ file in \"$DFG\" ] file using 1:4 notitle" >> .gnuplot_script # hamming distance of the hamming distances

    echo "set title \"Generations vs Percentage of Unique Fitnesses ($filename) \"" >> .gnuplot_script
    echo 'set xlabel "Generation"'  >> .gnuplot_script
    echo 'set ylabel "Percentage of Unique Fitnesses"' >> .gnuplot_script
echo "set output \"eps/percentage_unique/$filename.eps\"" >> .gnuplot_script
    echo "plot for [ file in \"$DFG\" ] file using 1:5 notitle" >> .gnuplot_script # percentage of unique individuals   
            
###################################################################################

    # make the png files
    echo "set terminal png enhanced" >> .gnuplot_script
    
    echo "set title \"Generations vs Standard Deviation ($filename) \"" >> .gnuplot_script
    echo 'set xlabel "Generation"'  >> .gnuplot_script
    echo 'set ylabel "Standard Deviation"' >> .gnuplot_script
echo "set output \"pictures/standard_deviation/$filename.png\"" >> .gnuplot_script
    echo "plot for [ file in \"$DFG\" ] file using 1:2 notitle" >> .gnuplot_script # standard Deviation

    echo "set title \"Generations vs Sum of Haming Distances ($filename) \"" >> .gnuplot_script
    echo 'set xlabel "Generation"'  >> .gnuplot_script
    echo 'set ylabel "Sum of Haming Distances"' >> .gnuplot_script
echo "set output \"pictures/sum_haming/$filename.png\"" >> .gnuplot_script
    echo "plot for [ file in \"$DFG\" ] file using 1:3 notitle" >> .gnuplot_script # sum of the hamming distances

    echo "set title \"Generations vs Haming Distance of Haming Distances ($filename) \"" >> .gnuplot_script
    echo 'set xlabel "Generation"'  >> .gnuplot_script
    echo 'set ylabel "Haming Distance of Haming Distances"' >> .gnuplot_script
echo "set output \"pictures/haming_of_haming/$filename.png\"" >> .gnuplot_script
    echo "plot for [ file in \"$DFG\" ] file using 1:4 notitle" >> .gnuplot_script # hamming distance of the hamming distances

    echo "set title \"Generations vs Percentage of Unique Fitnesses ($filename) \"" >> .gnuplot_script
    echo 'set xlabel "Generation"'  >> .gnuplot_script
    echo 'set ylabel "Percentage of Unique Fitnesses"' >> .gnuplot_script
echo "set output \"pictures/percentage_unique/$filename.png\"" >> .gnuplot_script
    echo "plot for [ file in \"$DFG\" ] file using 1:5 notitle" >> .gnuplot_script # percentage of unique individuals 
    
    gnuplot .gnuplot_script

done

rm -rf .toGraph
rm .gnuplot_script
