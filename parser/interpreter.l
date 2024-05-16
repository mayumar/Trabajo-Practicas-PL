/*! 
  \file interpreter.l
  \brief Lexical or scanner file
*/
%{
#include <iostream>

/*********************/
/* NEW in example 4 */
/* atof */
#include <stdlib.h>
/*********************/

/* Macros for the screen */
#include "../includes/macros.hpp"

/*********************/
/* 
  NEW in example 16 
  AST class
  IMPORTANT: must be written before interpreter.tab.h
*/
#include "../ast/ast.hpp"
/*********************/

/*  interpreter.tab.h contains the numeric values of the tokens produced by the parser */
#include "interpreter.tab.h"

/********************/
/* NEW in example 3 */
#include "../error/error.hpp"
/********************/


/*********************************************/
/* NEW in example 7 */
/* Table of symbols */
#include "../table/table.hpp"

#include "../table/numericVariable.hpp"
#include "../table/stringVariable.hpp"

extern lp::Table table; //!< External Table of Symbols
/*********************************************/

extern int lineNumber; //!< External line counter

/* uso de strdup */
#include <string.h>


%}


/*! \name REGULAR DEFINITIONS  */

DIGIT  [0-9] 

CHAR   [a-zA-Z]

NUMBER [+\-]?{DIGIT}+(\.{DIGIT}+)?((E|e)[+\-]?{DIGIT}+)?

IDENTIFIER {CHAR}({CHAR}|{DIGIT}|\_({CHAR}|{DIGIT}))*

STRING \'([^'\\]|\\.)*\'

/* NEW in example 3 */
/* STATE  */
%x ERROR COMMENT

/*! \name REGULAR EXPRESSIONS  */
%%

[ \t] 	{ ; }    /* skip white space and tabular */

\n      { 
			/* Line counter */
           	lineNumber++; 
	
			/* MODIFIED in example 3 */
			/* COMMENTED in example 5 */
        	/* return NEWLINE; */
        }

";"		{
			/* NEW in example 5 */
			return SEMICOLON;
		}


","		{
			/* NEW in example 14 */
			return COMMA;
		}

{NUMBER}  			  {
						/* MODIFIED in example 4 */
						/* Conversion of type and sending of the numerical value to the parser */
						yylval.number = atof(yytext); 

						return NUMBER; 
                      }

{STRING} 			  {
						/* Remove quotation marks */
						yytext[yyleng-1] = '\0';
						yytext++;
						yyleng--;

						yylval.string = yytext;
						return STRING;
					  }

{IDENTIFIER}   		 {
						 /* NEW in example 7 */
						 std::string identifier(yytext);

						 for(int i = 0; identifier[i]; i++)
						 	identifier[i] = tolower(identifier[i]);

						/* 
                          strdup() function returns a pointer to a new string 
                          which  is a duplicate of the string yytext
                        */
						  yylval.string =  strdup(yytext);

						/* If the identifier is not in the table of symbols then it is inserted */	
						 if (table.lookupSymbol(identifier) == false)
							{
								/* 
								 The identifier is inserted into the symbol table 
								 as undefined Variable with value 0.0
								*/
								lp::NumericVariable *n = new lp::NumericVariable(identifier,VARIABLE,UNDEFINED,0.0);

								/* A pointer to the new NumericVariable is inserted into the table of symbols */
								table.installSymbol(n);

								return VARIABLE;
							}

						/* MODIFIED in example 11 */
						/* 
							If the identifier is in the table of symbols then its token is returned 
								The identifier can be a variable or a numeric constant
						*/
						else 
							{
								lp::Symbol	*s = table.getSymbol(identifier);

/*
								std::cout << "lex: "<< s->getName() 
										  << "token " << s->getToken()
  										  << std::endl; 
*/

								/*  If the identifier is in the table then its token is returned  */
								return s->getToken();
							}
                     }

":="    { return ASSIGNMENT; }

"-"     { return MINUS;  } 	
"+"	    { return PLUS;   } 	
"*"	    { return MULTIPLICATION; }
"/"     { return DIVISION; } 
"//"    { return INT_DIVISION; }
"%"     { return MODULE; }
"**"    { return POWER; }
 
"||"    { return CONCAT; }

"<"		{ return LESS_THAN; }
"<="	{ return LESS_OR_EQUAL; }
">"		{ return GREATER_THAN; }
">="	{ return GREATER_OR_EQUAL; }
"="	    { return EQUAL; }
"<>"	{ return NOT_EQUAL; }

"("		{ return LPAREN; }
")"		{ return RPAREN; }

":"		{ return COLON; }

!!.* 	{ }

"<<" 	{ BEGIN(COMMENT); }

<COMMENT>[^">>"] { }

<COMMENT>">"|"<" { }

<COMMENT>">>" { BEGIN(INITIAL); }

<<EOF>> { /* The interpreter finishes when finds the end of file character */
		/*  PLACE(24,10);
	  	  std::cout <<  BICYAN;
	   	  std::cout <<  ">>>>>>> End of program <<<<<<<";
	 	  std::cout << std::endl;
	   	  std::cout << RESET;
		  PLACE(25,1);
		*/
	  	  return 0;
	}


.								{ 	
									/* Any other character */
									/* MODIFIIED in example 3 */
									/* Change to error state */
									BEGIN(ERROR); 

									/* 
										The current value of yytext 
										will be concatenated with the next character
									*/
									yymore();  
								}

<ERROR>[^0-9+\-*/()\^% \t\n\;a-zA-Z=<>!&]	{ /* MODIFIED in examples 5, 7, 15 */
								  /* NEW in example 3 */
								  /* 
									The current value of yytext 
									will be concatenated with the next character
								  */
									yymore(); 
								}

<ERROR>(.|\n)               { 
								/* NEW in example 3 */
								/* Rewind one position */
								yyless(yyleng-1); 

								/* Display the error message */
								warning("Lexical error", yytext);

								/* Restore the initial state */
   								BEGIN(INITIAL);
							}
%%

