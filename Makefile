###############################################################################
# 	Makefile
#
# A genetic algorithm for the optimization of task scheduling
# 
# AUTHOR : Jennifer Winer
#
# CREATED : May 21, 2013
# LAST MODIFIED : July 14, 2013
###############################################################################

###############################################################################
##
## Description:
## This makefile builds the Genetic Algorithm
##
## targets:
##  1. <default> : compile incrementally and link with optimization
##		    print only the final generation
##
##  2. debug	 : no optimization, enable profiling and valgrind options.
##		    enable debug printing functions in the code
##
##  3. verbose	 : print every generation
##
##  4. stats	 : print ever generation alongside its statistical
##		    information
##
###############################################################################

#compiler options
CC		= gcc
C_FLAGS 	= -std=gnu99 -Wall -pedantic

DEBUG_FLAGS	= -DDEBUG -g -p -O0
VERBOSE_FLAGS	= -DVERBOSE -O3
EXE_FLAGS	= -DEXE -O3 
DIVERSTITY_FLAGS= -DDIVERSITY -O3

C_INCLUDES   	= -Iinclude -Ilibs

L_INCLUDES	= -Llibs -lrcsSimulator -lofflineScheduler \
		  -lcommonInterfaces -lm 


#directory names
SRC_DIR		= src
OBJ_DIR		= obj
BIN_DIR		= .


OBJS		= $(addprefix $(OBJ_DIR)/, \
			fitness.o \
			individual.o \
			main.o \
			population.o \
			replacement.o \
			selection.o \
			util.o)


PROG_NAME	= Genetic_Algorithm.exe

###############################################################################
#      compilation, linking and debugging targets
###############################################################################


all : $(PROG_NAME)
	@ echo "Build updated"
	
clean : 
	rm -f $(BIN_DIR)/$(PROG_NAME)
	rm -f $(OBJ_DIR)/*

run: 
	./$(PROG_NAME) 
	
###############################################################################

.PHONY : $(PROG_NAME) 
	
# Creates different versions of the code
debug   : C_FLAGS += $(DEBUG_FLAGS)
debug   : L_INCLUDES += -p
debug   : $(PROG_NAME)
	
verbose : C_FLAGS += $(VERBOSE_FLAGS)
verbose : $(PROG_NAME)
	
diversity   : C_FLAGS += $(DIVERSTITY_FLAGS) 
diversity   : $(PROG_NAME)

###############################################################################

$(PROG_NAME) : $(OBJS)
	$(CC) $(OBJS) -o $(PROG_NAME) $(L_INCLUDES) 
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(OBJ_DIR) 
	$(CC) $(C_INCLUDES) $(C_FLAGS) -c $< -o $@
    
$(OBJ_DIR) : 
	mkdir -p $(OBJ_DIR)
