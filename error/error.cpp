/*! 
  \file error.cpp
  \brief Code of error recovery functions 
*/


// cerr, endl
#include <iostream>

#include <string>

/*  longjmp */
#include <setjmp.h>

// ERANGE, EDOM
#include <errno.h>

#include "error.hpp"

// Macros for the screen
#include "../includes/macros.hpp"

// For kill proccess
#include <cstdlib>

extern int lineNumber; //!< // Reference to line counter

extern std::string progname; //!<  Reference to program name

extern jmp_buf begin; //!< Used for error recovery 

// NEW
extern int errno; //!<  ReferenceReference to the global variable that controls errors in the mathematical code


void warning(std::string errorMessage1,std::string errorMessage2)
{
  std::cerr << IGREEN; 
  std::cerr << " Programa: " << progname << std::endl;
  std::cerr << BIRED; 
  std::cerr << " Error en la linea " << lineNumber 
            << " --> " << errorMessage1 << " ";
  std::cerr << RESET; 


  if (errorMessage2.compare("")!=0)
		 std::cerr << errorMessage2 << std::endl;
}

void yyerror(std::string errorMessage)
{
	warning("Error de análisis",errorMessage);
  std::exit(-1);
}


void execerror(std::string errorMessage1,std::string errorMessage2)
{
  warning(errorMessage1,errorMessage2); 
  longjmp(begin,0); /* return to a viable state */
  std::exit(-1);
}

void fpecatch(int signum)     
{
 execerror("Tiempo de ejecución","error de punto flotante");
}



double errcheck(double d, std::string s)
{
  if (errno==EDOM)
    {
     errno=0;
     std::string msg("Error de tiempo de ejecución --> argumento fuera de dominio");
 
     std::cout << msg << std::endl;
     execerror(s,msg);
    }
   else if (errno==ERANGE)
           {
		 	std::string msg("Error de tiempo de ejecución --> resultado fuera de rango");
            errno=0;
            execerror(s,msg);
           }

 return d;
}



