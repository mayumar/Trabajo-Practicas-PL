/*! 
  \file interpreter.l
  \brief Lexical or scanner file
*/

%{
#include <iostream>

/*********************/
/* atof */
#include <stdlib.h>
/*********************/

/* Macros for the screen */
#include "../includes/macros.hpp"

/*  interpreter.tab.h contains the numeric values of the tokens produced by the parser */
#include "interpreter.tab.h"

/********************/
#include "../error/error.hpp"
/********************/

extern int lineNumber; //!< External line counter

%}

/*! \name REGULAR DEFINITIONS  */

DIGIT  [0-9]
CHAR   [a-zA-Z]

NUMBER {DIGIT}+(\.{DIGIT}+)?(E[+\-]?{DIGIT}+)?
IDENTIFIER {CHAR}({CHAR}|{DIGIT}|\_({CHAR}|{DIGIT}))*

/* STATE  */
%x ERROR

/*! \name REGULAR EXPRESSIONS  */
%%

[ \t] 	{ ; }    /* skip white space and tabulator */

\n { 
	/* Line counter */
	lineNumber++;
}

";"	{ return SEMICOLON; }

{NUMBER}  {
	/* Conversion of type and sending of the numerical value to the parser */
	yylval.number = atof(yytext); 
	return NUMBER; 
}


"#o"    { /* return OR; */ }
"#y"    { /* return AND; */ }
"#no"   { /* return NOT; */ }

":="    { /* return ASSIGNMENT; */ }

"-"     { return MINUS;  } 	
"+"	    { return PLUS;   } 	
"*"	    { return MULTIPLICATION; }
"/"     { return DIVISION; } 
"//"    { return INT_DIVISION; }
"%"     { return MODULE; }
"**"    { return POWER; }
 
"||"    { /* return CONCAT; */ }

"<"		{ /* return LESS_THAN; */ }
"<="	{ /* return LESS_OR_EQUAL; */ }
">"		{ /* return GREATER_THAN; */ }
">="	{ /* return GREATER_OR_EQUAL; */ }
"="	    { /* return EQUAL; */ }
"<>"	{ /* return NOT_EQUAL; */ }

"("		{ return LPAREN; }
")"		{ return RPAREN; }

{IDENTIFIER} { return IDENTIFIER; }

<<EOF>> { /* The interpreter finishes when finds the end of file character */
		 // PLACE(24,10);
	  	  std::cout <<  BICYAN;
	   	  std::cout <<  ">>>>>>> End of program <<<<<<<";
	 	  std::cout << std::endl;
	   	  std::cout << RESET;
		 // PLACE(25,1);

	  	  return 0;
	}

.								{ 	
									/* Any other character */
									/* Change to error state */
									BEGIN(ERROR); 

									/* 
										The current value of yytext 
										will be concatenated with the next character
									*/
									yymore();  
								}

<ERROR>[^0-9+\-*/()\^% \t\n\;]	{ 
								  /* 
									The current value of yytext 
									will be concatenated with the next character
								  */
									yymore(); 
								}

<ERROR>(.|\n)                {  
								/* Rewind one position */
								yyless(yyleng-1); 

								/* Display the error message */
								warning("Lexical error", yytext);

								/* Restore the initial state */
   								BEGIN(INITIAL);
						}
%%
