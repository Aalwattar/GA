=GA=
Implementation of a very simple integer based Genetic Algorithm


== Running the Program ==
GeneticAlgorithm.exe <options>

* -c DECIMAL_NUM
*: Set the crossover rate to DECIMAL_NUM
    
* -g INTEGER
*: Run the genetic algorithm for INTEGER generations
    
* -i STRING
*: The name of the input file

* -m DECIMAL_NUM
*: Set the mutation rate to DECIMAL_NUM

* -p INTEGER
*: Set the Population size to INTEGER individuals

* -t INTEGER
*: Seed the random number generator with INTEGER

These arguments are all optional and can be entered in any order.

more options are currently in development


=== Default Values ===
* input file = input/problem.txt

* Crossover rate = 0.85
* Mutation rate = 0.005

* Population size = 50
* Number of Generations = 500


=== Program Output ===
Error messages are currently being printed to stderr.
All other program output is printed to stdout.


=== Constraints === 
* The Crossover rate should be a decimal number between 0 and 1
* The Mutation rate should be a decimal number between 0 and 1

* The Population size should be a number greater than 2
* The number of generations should be greater than 1


== FUTURE IMPLEMENTATION IDEAS == 
Please see comments // TODO and // FIXME in the source code for future implementation ideas
