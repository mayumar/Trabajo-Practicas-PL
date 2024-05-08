/*! 
  \file interpreter.y
  \brief Grammar file
*/


%{
#include <iostream>
#include <string>

/*******************************************/
#include <math.h>
/*******************************************/

/*******************************************/
/* Use for recovery of runtime errors */
#include <setjmp.h>
#include <signal.h>
/*******************************************/

/* Error recovery functions */
#include "../error/error.hpp"

/* Macros for the screen */
#include "../includes/macros.hpp"

/*! 
	\brief  Lexical or scanner function
	\return int
	\note   C++ requires that yylex returns an int value
	\sa     yyparser
*/
int yylex();


extern int lineNumber; //!< External line counter

/***********************************************************/
extern std::string progname; //!<  Program name
/***********************************************************/

/*******************************************/
/*
 jhmp_buf
    This is an array type capable of storing the information of a calling environment to be restored later.
   This information is filled by calling macro setjmp and can be restored by calling function longjmp.
*/
jmp_buf begin; //!<  It enables recovery of runtime errors 
/*******************************************/

#define ERROR_BOUND 1.0e-6  //!< To compare real numbers


%}

/* In case of a syntactic error, more information is shown */
/* DEPRECATED */
/* %error-verbose */

/* ALTERNATIVA a %error-verbose */
%define parse.error verbose

//! \name Grammar symbols

/* Initial grammatical symbol */
%start program 

/*******************************************/
/* Data type YYSTYPE  */
%union
{
  double number;  
}

/* Data type of the non-terminal symbol "exp" */
%type <number> exp
/*******************************************/

/* Defined token */

/* Minimum precedence */

/*******************************************/
%token SEMICOLON
/*******************************************/

/*******************************************/
%token <number> NUMBER
/*******************************************/

/* Left associativity */

/*******************************************/
%left PLUS MINUS 

%left MULTIPLICATION DIVISION INT_DIVISION MODULE

%left LPAREN RPAREN

%nonassoc  UNARY

/* Maximum precedence  */
/* NEW in example 5 */
%right POWER
/*******************************************/

%%
//! \name Grammar rules

program : stmtlist {  }
;

		/* MODIFIED in example 5: SEMICOLON */ 
stmtlist:  /* empty: epsilon rule */

		| stmtlist SEMICOLON     /* empty line */

		/* MODIFIED in example 4: the result of the expression is displayed  */ 
        | stmtlist exp SEMICOLON
            { 
				std::cout << BIYELLOW; 
				std::cout << "Result: ";
				std::cout << RESET; 
				std::cout << $2 << std::endl << std::endl;
            }

        | stmtlist error SEMICOLON
			{
				// The function yyerror is called
				// If %error-verbose is used then an error message is displayed
			}
;

exp: 	NUMBER
		{ $$ = $1; }

	|	exp PLUS exp
		{ $$ = $1 + $3; }

	|	PLUS exp %prec UNARY
		{ $$ = +$2; }

	|	exp MINUS exp
		{ $$ = $1 - $3; }

	|	MINUS exp %prec UNARY
		{ $$ = -$2; }

	|	exp MULTIPLICATION exp
		{ $$ = $1 * $3; }

	|	exp DIVISION exp
		{   
			if (fabs($3) < ERROR_BOUND) 
  				execerror("Runtime error in division", "Division by zero");
			else 
    	        $$ = $1 / $3;
		}

	|	exp INT_DIVISION exp
		{
			if(fabs($3) < ERROR_BOUND)
				execerror("Runtime error in division", "Division by zero");
			else
				$$ = (int) $1 / (int) $3;
			
		}

	|	LPAREN exp RPAREN
		{ $$ = $2; }

	|	exp MODULE exp 
		{
			if (fabs($3) < ERROR_BOUND)  
				execerror("Runtime error in modulo", "Division by zero");
			else 
				$$ = (int) $1 % (int) $3;
       }
	
	|	exp POWER exp 
     	{ $$ = pow($1,$3); }

;
 
%%


