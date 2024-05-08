NAME=interpreter

# Compiler
CPP = g++

# Directives for the compiler
# -c: the output is an object file, the linking stage is not done.
# -g: debug
# -Wall: all warnings
# -ansi: standard language
# -02: optimization level
CFLAGS = -c -g  -Wall -ansi -O2

# Directive for flex's library
LFLAGS = -lfl
#LFLAGS = -ll

# Objects
OBJECTS= $(NAME).o
OBJECTS-PARSER = parser/*.o
OBJECTS-ERROR = error/*.o
OBJECTS-TABLE = table/*.o


# Includes
INCLUDES = ./parser/interpreter.tab.h ./error/error.hpp \
			./table/table.hpp

#######################################################
# Predefined macros 
#
# $@: name of the target
# $^: all the dependencies
# $<: first dependency
#
#######################################################

$(NAME).exe :  parser-dir error-dir table-dir $(OBJECTS)
	@echo "Generating " $@
	@$(CPP) $(OBJECTS) $(OBJECTS-PARSER) $(OBJECTS-ERROR)  $(OBJECTS-TABLE) $(LFLAGS) -o $@

# Main program
$(NAME).o: $(NAME).cpp parser-dir $(INCLUDES)
	@echo "Compiling " $<
	@$(CPP) $(CFLAGS) $<
	@echo

# 
parser-dir:
	@echo "Accessing directory parser" 
	@echo
	@make -C parser/
	@echo

error-dir:
	@echo "Accessing directory error" 
	@echo
	@make -C error/ 
	@echo

table-dir:
	@echo "Accessing directory table" 
	@echo
	@make -C table/ 
	@echo

#######################################################
$(NAME).output: 
	@echo "Generating: " $@
	@make -C parser/ $@
	@echo


#######################################################
# Generate the documentation
doc: Doxyfile
	doxygen

#######################################################
# Auxiliary files and hmtl directory are deleted
clean: 
	@echo "Deleting html"
	@rm -rf html 
	@echo "Deleting " $(OBJECTS)  $(NAME).exe  *~ 
	@rm -f $(OBJECTS) $(NAME).exe *~
	@echo
	@make -C parser/ clean 
	@echo
	@make -C error/ clean
	@echo
	@make -C table/ clean
	@echo



