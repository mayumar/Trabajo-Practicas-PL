/*! 
  \file interpreter.y
  \brief Grammar file
*/


%{
#include <iostream>
#include <string>

/*******************************************/
/* pow */
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


/*******************************************/
/* 
  AST class
  IMPORTANT: this file must be before init.hpp
*/
#include "../ast/ast.hpp"


/*******************************************/
/* Table of symbol */
#include "../table/table.hpp"
/*******************************************/

/*******************************************/
#include "../table/numericVariable.hpp"
/*******************************************/

/*******************************************/
#include "../table/stringVariable.hpp"
/*******************************************/

#include "../table/logicalVariable.hpp"

/*******************************************/
#include "../table/numericConstant.hpp"
/*******************************************/

/*******************************************/
#include "../table/logicalConstant.hpp"
/*******************************************/

/*******************************************/
#include "../table/builtinParameter1.hpp"
/*******************************************/

/*******************************************/
#include "../table/builtinParameter0.hpp"
#include "../table/builtinParameter2.hpp"
/*******************************************/


/*******************************************/
#include "../table/init.hpp"
/*******************************************/

/*! 
	\brief  Lexical or scanner function
	\return int
	\note   C++ requires that yylex returns an int value
	\sa     yyparser
*/
int yylex();


extern int lineNumber; //!< External line counter


extern bool interactiveMode; //!< Control the interactive mode of execution of the interpreter

extern int control; //!< External: to control the interactive mode in "if" and "while" sentences 




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


/*******************************************/
extern lp::Table table; //!< Extern Table of Symbols

/*******************************************/
extern lp::AST *root; //!< External root of the abstract syntax tree AST

%}

/* In case of a syntactic error, more information is shown */
/* DEPRECATED */
/* %error-verbose */

/* ALTERNATIVA a %error-verbose */
%define parse.error verbose


/* Initial grammar symbol */
%start program

/*******************************************/
/* Data type YYSTYPE  */
%union {
  double number;
  char * string; 				 
  bool logic;						 
  lp::ExpNode *expNode;  			 
  std::list<lp::ExpNode *>  *parameters;    // NOTE: #include<list> must be in interpreter.l, init.cpp, interpreter.cpp
  std::list<lp::Statement *> *stmts; 
  lp::Statement *st;				 
  std::list<lp::CaseStmt *> *cases;
  lp::AST *prog;			
}

/* Type of the non-terminal symbols */
%type <expNode> exp cond 

%type <parameters> listOfExp  restOfListOfExp

%type <stmts> stmtlist

%type <st> stmt asgn print read read_string if while repeat for switch clear place do_while plusasgn minusasgn plusplus minusminus

%type <prog> program

%type <cases> caselist

/* Defined tokens */

/* Minimum precedence */

/*******************************************/
%token SEMICOLON
/*******************************************/

%token READ READ_STRING PRINT IF THEN ELSE END_IF WHILE DO END_WHILE REPEAT UNTIL FOR END_FOR FROM STEP SWITCH CASE DEFAULT END_SWITCH QUESTION_MARK

%right ASSIGNMENT PLUSASSIGNMENT MINUSASSIGNMENT

%token COMMA

%token COLON

%token CLEAR PLACE

/*******************************************/
%token <number> NUMBER
/*******************************************/
%token <logic> BOOL
/*******************************************/
%token <string> VARIABLE STRING UNDEFINED CONSTANT BUILTIN

/* Left associativity */

/*******************************************************/
%left OR

%left AND

%nonassoc GREATER_OR_EQUAL LESS_OR_EQUAL GREATER_THAN LESS_THAN  EQUAL NOT_EQUAL

%left NOT
/*******************************************************/

%left PLUS MINUS

%left MULTIPLICATION DIVISION INT_DIVISION MODULE CONCAT

%left PLUSPLUS MINUSMINUS

%left LPAREN RPAREN

%nonassoc  UNARY

// Maximum precedence 
%right POWER


%%
 //! \name Grammar rules


program : stmtlist
		  { 
		    // Create a new AST
			$$ = new lp::AST($1); 

			// Assign the AST to the root
			root = $$; 

			// End of parsing
			//	return 1;
		  }
;

stmtlist:  /* empty: epsilon rule */
		  { 
			// create a empty list of statements
			$$ = new std::list<lp::Statement *>(); 
		  }  

        | stmtlist stmt 
		  { 
			// copy up the list and add the stmt to it
			$$ = $1;
			$$->push_back($2);

			// Control the interative mode of execution of the interpreter
			if (interactiveMode == true && control == 0)
 			{
				for(std::list<lp::Statement *>::iterator it = $$->begin(); 
						it != $$->end(); 
						it++)
				{
					(*it)->printAST();
					(*it)->evaluate();
					
				}

				// Delete the AST code, because it has already run in the interactive mode.
				$$->clear();
			}
		}

    | stmtlist error 
      { 
			 // just copy up the stmtlist when an error occurs
			 $$ = $1;

			 // The previous look-ahead token ought to be discarded with `yyclearin;'
			 yyclearin; 
       } 
;
 

stmt: SEMICOLON  /* Empty statement: ";" */
	  {
		// Create a new empty statement node
		$$ = new lp::EmptyStmt(); 
	  }
	| asgn  SEMICOLON
	  {
		// Default action
		// $$ = $1;
	  }
	| print SEMICOLON
	  {
		// Default action
		// $$ = $1;
	  }
	| read SEMICOLON
	  {
		// Default action
		// $$ = $1;
	  }
	| read_string SEMICOLON
	  {
		// Default action
		// $$ = $1;
	  }
	| if SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }
	| while SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }
	/* Repeat statement */
	| repeat SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }
	/* For statement */
	| for SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }
	/* Switch statement */
	| switch SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }

	| clear SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }

	| place SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }

	| do_while SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }

	| plusasgn SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }

	| minusasgn SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }

	| plusplus SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }

	| minusminus SEMICOLON
	 {
		// Default action
		// $$ = $1;
	 }
;

controlSymbol:  /* Epsilon rule*/
		{
			// To control the interactive mode in "if" and "while" sentences
			control++;
		}
	;

if:	/* Simple conditional statement */
	IF controlSymbol cond THEN stmtlist END_IF
    {
		// Create a new if statement node
		$$ = new lp::IfStmt($3, new lp::BlockStmt($5));

		// To control the interactive mode
		control--;
	}

	/* Compound conditional statement */
	| IF controlSymbol cond THEN stmtlist ELSE stmtlist END_IF
	 {
		// Create a new if statement node
		$$ = new lp::IfStmt($3, new lp::BlockStmt($5), new lp::BlockStmt($7));

		// To control the interactive mode
		control--;
	 }
;

while:  WHILE controlSymbol cond DO stmtlist END_WHILE 
	{
		// Create a new while statement node
		$$ = new lp::WhileStmt($3, new lp::BlockStmt($5));

		// To control the interactive mode
		control--;
    }
;

repeat: REPEAT stmtlist UNTIL controlSymbol cond
	{
		$$ = new lp::RepeatStmt(new lp::BlockStmt($2), $5);

		control--;
	}
;

for: FOR controlSymbol VARIABLE FROM exp UNTIL exp STEP exp DO stmtlist END_FOR
	{
		$$ = new lp::ForStmt($3, $5, $7, $9, new lp::BlockStmt($11));

		control--;
	}

	| FOR controlSymbol VARIABLE FROM exp UNTIL exp DO stmtlist END_FOR
	 {
		$$ = new lp::ForStmt($3, $5, $7, new lp::BlockStmt($9));

		control--;
	 }
;

switch: SWITCH controlSymbol LPAREN exp RPAREN caselist END_SWITCH
	 {
		$$ = new lp::SwitchStmt($4, $6);

		control--;
	 }

	| SWITCH controlSymbol LPAREN exp RPAREN caselist DEFAULT COLON stmtlist END_SWITCH
	 {
		$$ = new lp::SwitchStmt($4, $6, new lp::BlockStmt($9));

		control--;
	 }
;

caselist: //Epsilon rule
	 {
		$$ = new std::list<lp::CaseStmt *>();
	 }
	 
	| caselist CASE exp COLON stmtlist
	 {
		$$ = $1;
		$$->push_back(new lp::CaseStmt($3, new lp::BlockStmt($5)));
	 }
;

cond: 	LPAREN exp RPAREN
		{ 
			$$ = $2;
		}
;


asgn:   VARIABLE ASSIGNMENT exp 
		{ 
			// Create a new assignment node
			$$ = new lp::AssignmentStmt($1, $3);
		}

	|  VARIABLE ASSIGNMENT asgn 
		{ 
			// Create a new assignment node
			$$ = new lp::AssignmentStmt($1, (lp::AssignmentStmt *) $3);
		}

	| CONSTANT ASSIGNMENT exp 
		{   
 			execerror("Error semántico en asignación: no está permitido modificar una constante ", $1);
		}
	| CONSTANT ASSIGNMENT asgn 
		{   
 			execerror("Error semántico en múltiples asignaciones: no está permitido modificar una constante ",$1);
		}
;

plusasgn: VARIABLE PLUSASSIGNMENT exp 
		{ 
			// Create a new assignment node
			$$ = new lp::PlusAssignmentStmt($1, $3);
		}

	| CONSTANT PLUSASSIGNMENT exp 
		{   
 			execerror("Error semántico en asignación: no está permitido modificar una constante ", $1);
		}
;

minusasgn: VARIABLE MINUSASSIGNMENT exp 
		{ 
			// Create a new assignment node
			$$ = new lp::MinusAssignmentStmt($1, $3);
		}

	| CONSTANT MINUSASSIGNMENT exp 
		{   
 			execerror("Error semántico en asignación: no está permitido modificar una constante ", $1);
		}
;

print:  PRINT LPAREN exp RPAREN
		{
			// Create a new print node
			 $$ = new lp::PrintStmt($3);
		}
;	

read:  READ LPAREN VARIABLE RPAREN  
		{
			// Create a new read node
			 $$ = new lp::ReadStmt($3);
		}

	| READ LPAREN CONSTANT RPAREN  
		{   
 			execerror("Error semántico en \"leer()\": no está permitido modificar una constante ",$3);
		}
;

read_string:  READ_STRING LPAREN VARIABLE RPAREN  
		{
			// Create a new read_string node
			 $$ = new lp::ReadStringStmt($3);
		}

	| READ_STRING LPAREN CONSTANT RPAREN  
		{   
 			execerror("Error semántico en \"leer_cadena()\": no está permitido modificar una constante ",$3);
		}
;

clear: CLEAR
		{
			$$ = new lp::ClearStmt();
		}
;

place: PLACE LPAREN exp COMMA exp RPAREN
		{
			$$ = new lp::PlaceStmt($3, $5);
		}
;

do_while: DO stmtlist WHILE controlSymbol cond
	{
		$$ = new lp::DoWhileStmt(new lp::BlockStmt($2), $5);

		control--;
	}
;

plusplus: VARIABLE PLUSPLUS %prec UNARY
	{
		$$ = new lp::PlusPlusStmt($1);
	}
;

minusminus: VARIABLE MINUSMINUS %prec UNARY
	{
		$$ = new lp::MinusMinusStmt($1);
	}
;

exp:	NUMBER 
		{ 
			// Create a new number node
			$$ = new lp::NumberNode($1);
		}

	| 	exp PLUS exp 
		{ 
			// Create a new plus node
			$$ = new lp::PlusNode($1, $3);
		}

	| 	exp MINUS exp
      	{
			// Create a new minus node
			$$ = new lp::MinusNode($1, $3);
		}

	| 	exp MULTIPLICATION exp 
		{ 
			// Create a new multiplication node
			$$ = new lp::MultiplicationNode($1, $3);
		}

	| 	exp DIVISION exp
		{
		  // Create a new division node	
		  $$ = new lp::DivisionNode($1, $3);
	   }

	|	exp INT_DIVISION exp
		{
			$$ = new lp::IntDivisionNode($1, $3);
		}

	|	exp CONCAT exp
		{
			$$ = new lp::ConcatNode($1, $3);
		}

	| 	LPAREN exp RPAREN
       	{ 
		    // just copy up the expression node 
			$$ = $2;
		 }

  	| 	PLUS exp %prec UNARY
		{ 
		  // Create a new unary plus node	
  		  $$ = new lp::UnaryPlusNode($2);
		}

	| 	MINUS exp %prec UNARY
		{ 
		  // Create a new unary minus node	
  		  $$ = new lp::UnaryMinusNode($2);
		}

	|	exp MODULE exp 
		{
		  // Create a new modulo node	

		  $$ = new lp::ModuloNode($1, $3);
		}

	|	exp POWER exp 
     	{ 
		  // Create a new power node	
  		  $$ = new lp::PowerNode($1, $3);
		}

	|  	VARIABLE PLUSPLUS %prec UNARY
		{
			$$ = new lp::PlusPlusNode($1);
		}

	|  	VARIABLE MINUSMINUS %prec UNARY
		{
			$$ = new lp::MinusMinusNode($1);
		}

	|	VARIABLE
		{
		  // Create a new variable node	
		  $$ = new lp::VariableNode($1);
		}

	| 	STRING
		{
		  // Create a new string node
		  $$ = new lp::StringNode($1);
		}

	| 	CONSTANT
		{
		  // Create a new constant node	
		  $$ = new lp::ConstantNode($1);
		}

	| 	BUILTIN LPAREN listOfExp RPAREN
		{
			// Get the identifier in the table of symbols as Builtin
			lp::Builtin *f= (lp::Builtin *) table.getSymbol($1);

			// Check the number of parameters 
			if (f->getNParameters() ==  (int) $3->size())
			{
				switch(f->getNParameters())
				{
					case 0:
						{
							// Create a new Builtin Function with 0 parameters node	
							$$ = new lp::BuiltinFunctionNode_0($1);
						}
						break;

					case 1:
						{
							// Get the expression from the list of expressions
							lp::ExpNode *e = $3->front();

							// Create a new Builtin Function with 1 parameter node	
							$$ = new lp::BuiltinFunctionNode_1($1,e);
						}
						break;

					case 2:
						{
							// Get the expressions from the list of expressions
							lp::ExpNode *e1 = $3->front();
							$3->pop_front();
							lp::ExpNode *e2 = $3->front();

							// Create a new Builtin Function with 2 parameters node	
							$$ = new lp::BuiltinFunctionNode_2($1,e1,e2);
						}
						break;

					default:
				  			 execerror("Error de sintaxis: demasiados parámetros para la función ", $1);
				} 
			}
			else
	  			 execerror("Error de sintaxis: numero de parámetros incompatibles para la función", $1);
		}

	| exp GREATER_THAN exp
	 	{
		  // Create a new "greater than" node	
 			$$ = new lp::GreaterThanNode($1,$3);
		}

	| exp GREATER_OR_EQUAL exp 
	 	{
		  // Create a new "greater or equal" node	
 			$$ = new lp::GreaterOrEqualNode($1,$3);
		}

	| exp LESS_THAN exp 	
	 	{
		  // Create a new "less than" node	
 			$$ = new lp::LessThanNode($1,$3);
		}

	| exp LESS_OR_EQUAL exp 
	 	{
		  // Create a new "less or equal" node	
 			$$ = new lp::LessOrEqualNode($1,$3);
		}

	| exp EQUAL exp 	
	 	{
		  // Create a new "equal" node	
 			$$ = new lp::EqualNode($1,$3);
		}

    | exp NOT_EQUAL exp 	
	 	{
		  // Create a new "not equal" node	
 			$$ = new lp::NotEqualNode($1,$3);
		}

    | exp AND exp 
	 	{
		  // Create a new "logic and" node	
 			$$ = new lp::AndNode($1,$3);
		}

    | exp OR exp 
	 	{
		  // Create a new "logic or" node	
 			$$ = new lp::OrNode($1,$3);
		}

    | NOT exp 
	 	{
		  // Create a new "logic negation" node	
 			$$ = new lp::NotNode($2);
		}

	| LPAREN cond QUESTION_MARK exp COLON exp RPAREN
		{ 
			$$ = new lp::TernaryNode($2, $4, $6);
		}
;


listOfExp: 
			/* Empty list of numeric expressions */
			{
			    // Create a new list STL
				$$ = new std::list<lp::ExpNode *>(); 
			}

	|  exp restOfListOfExp
			{
				$$ = $2;

				// Insert the expression in the list of expressions
				$$->push_front($1);
			}
;

restOfListOfExp:
			/* Empty list of numeric expressions */
			{
			    // Create a new list STL
				$$ = new std::list<lp::ExpNode *>(); 
			}

		|	COMMA exp restOfListOfExp
			{
				// Get the list of expressions
				$$ = $3;

				// Insert the expression in the list of expressions
				$$->push_front($2);
			}
;



%%