###############################################################################
# 	Makefile
#
# An off-line Task Scheduler for FPGA
# 
# AUTHOR : Ziad Abuowaimer & Jennifer Winer
#
# CREATED : May 21, 2013
# LAST MODIFIED : May 8, 2014
###############################################################################

###############################################################################
##
## Description:
## This makefile builds the Off-Line Scheduler
##
## targets:
##	1. <default>    : compile incrementally and link
##	2. rebuild	: compile all and link
##
##	Set DEBUG="-g -D__DEBUG" to compile the source files for debugging
##	and enable the test jigs in the code.
##
###############################################################################

#compiler options

CC				= gcc
C_FLAGS			= -std=c99 -Wall -pedantic

DEBUG_FLAGS		= -DDEBUG -g -p -O0
VERBOSE_FLAGS	= -DVERBOSE
EXE_FLAGS		= -DEXE -O2

C_INCLUDES		= -Iinclude
L_INCLUDES		= -lm

#directory names
SRC_DIR			= src
OBJ_DIR			= obj
BIN_DIR			= .

OBJS			= $(addprefix $(OBJ_DIR)/,
                          fitness.o \
                          individual.o \
                          main.o \
                          population.o \
                          replacement.o \
                          selection.o \
                          util.o)


PROG_NAME		= GeneticAlgorithm.exe

###############################################################################
#      compilation, linking and debugging targets
###############################################################################


all : $(PROG_NAME)
	@ echo "Build updated"
	
clean : 
	rm -f $(BIN_DIR)/$(PROG_NAME)
	rm -f $(OBJ_DIR)*

run: 
	./$(PROG_NAME)
	
###############################################################################

.PHONY : $(PROG_NAME) 
	
# Creates different versions of the code
debug   : C_FLAGS += $(DEBUG_FLAGS)
debug   : $(PROG_NAME)
	
verbose : C_FLAGS += $(VERBOSE_FLAGS)
verbose : $(PROG_NAME)
	
exe     : C_FLAGS += $(EXE_FLAGS)
exe     : $(PROG_NAME)

###############################################################################

$(PROG_NAME) : $(OBJS)
	$(CC) $(OBJS) $(L_INCLUDES) -o $(PROG_NAME)
	

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(OBJ_DIR) 
	$(CC) $(C_INCLUDES) $(C_FLAGS) -c $< -o $@
    
$(OBJ_DIR) : 
	mkdir -p $(OBJ_DIR)
