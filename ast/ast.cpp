/*!	
	\file    ast.cpp
	\brief   Code of functions of AST clas
	\author  
	\date    2018-12-13
	\version 1.0
*/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <list>
#include <sstream>
#include <string.h>

// Para usar la funciones pow y std::abs
#include <cmath>

#include "ast.hpp"

#include "../table/table.hpp"

// warning
#include "../error/error.hpp"

// Macros for the screen
#include "../includes/macros.hpp"


#include "../table/numericVariable.hpp"
#include "../table/stringVariable.hpp"
#include "../table/logicalVariable.hpp"

#include "../table/numericConstant.hpp"
#include "../table/logicalConstant.hpp"

#include "../table/builtinParameter0.hpp"
#include "../table/builtinParameter1.hpp"
#include "../table/builtinParameter2.hpp"

#include "../parser/interpreter.tab.h"



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

extern lp::Table table; //!< Reference to the Table of Symbols


extern lp::AST *root; //!< Reference to the object at the base of the AST


///////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::VariableNode::getType() 
{ 
	// Get the identifier in the table of symbols as Variable
	lp::Variable *var = (lp::Variable *) table.getSymbol(this->_id);

	// Return the type of the Variable
	return var->getType();
}


void lp::VariableNode::printAST() 
{
  std::cout << "VariableNode: " << this->_id;
  std::cout << " (Type: " << this->getType() << ")" << std::endl;
}


double lp::VariableNode::evaluateNumber() 
{ 
	double result = 0.0;

	if (this->getType() == NUMBER)
	{
		// Get the identifier in the table of symbols as NumericVariable
		lp::NumericVariable *var = (lp::NumericVariable *) table.getSymbol(this->_id);

		// Copy the value of the NumericVariable
		result = var->getValue();
	}
	else
	{
		warning("Error de tiempo de ejecucion: la variable no es numerica", 
				   this->_id);
	}

	// Return the value of the NumericVariable
	return result;
}

std::string lp::VariableNode::evaluateString()
{
	std::string result = "";

	if (this->getType() == STRING || this->getType() == NUMBER)
	{
		// Get the identifier in the table of symbols as StringVariable
		lp::StringVariable *var = (lp::StringVariable *) table.getSymbol(this->_id);

		// Copy the value of the StringVariable
		result = var->getValue();
	}
	else
	{
		warning("Error de tiempo de ejecucion: la variable no es alfanumerica", this->_id);
	}

	// Return the value of the StringVariable
	return result;
}


bool lp::VariableNode::evaluateBool() 
{ 
	bool result = false;

	if (this->getType() == BOOL)
	{
		// Get the identifier in the table of symbols as LogicalVariable
		lp::LogicalVariable *var = (lp::LogicalVariable *) table.getSymbol(this->_id);

		// Copy the value of the LogicalVariable
		result = var->getValue();
	}
	else
	{
		warning("Error de tiempo de ejecucion: la variable no es booleana",
				   this->_id);
	}

	// Return the value of the LogicalVariable
	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ConstantNode::printAST() 
{
  std::cout << "ConstantNode: " << this->_id;
  std::cout << " (Type: " << this->getType() << ")" << std::endl;
}

int lp::ConstantNode::getType() 
{ 
	// Get the identifier in the table of symbols as Constant
	lp::Constant *var = (lp::Constant *) table.getSymbol(this->_id);

	// Return the type of the Constant
	return var->getType();
}


double lp::ConstantNode::evaluateNumber() 
{ 
	double result = 0.0;

	if (this->getType() == NUMBER)
	{
		// Get the identifier in the table of symbols as NumericConstant
		lp::NumericConstant *constant = (lp::NumericConstant *) table.getSymbol(this->_id);

		// Copy the value of the NumericConstant
		result = constant->getValue();
	}
	else
	{
		warning("Error de tiempo de ejecucion: la constante no es numerica", 
				   this->_id);
	}

	// Return the value of the NumericVariable
	return result;
}

bool lp::ConstantNode::evaluateBool() 
{ 
	bool result = false;

	if (this->getType() == BOOL)
	{
		// Get the identifier in the table of symbols as LogicalConstant
		lp::LogicalConstant *constant = (lp::LogicalConstant *) table.getSymbol(this->_id);

		// Copy the value of the LogicalConstant
		result = constant->getValue();
	}
	else
	{
		warning("Error de tiempo de ejecucion: la constante no es booleana",
				   this->_id);
	}

	// Return the value of the LogicalVariable
	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
 

int lp::NumberNode::getType()
{
	return NUMBER;
}


void lp::NumberNode::printAST()
{
  std::cout << "NumberNode: " << this->_number << std::endl;
}

double lp::NumberNode::evaluateNumber() 
{ 
    return this->_number; 
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::StringNode::getType()
{
	return STRING;
}


void lp::StringNode::printAST()
{
//   std::cout << "StringNode: " << this->_string << std::endl;
}

std::string lp::StringNode::evaluateString() 
{ 
	std::string processedValue;
	for (size_t i = 0; i < this->_string.length(); i++)
	{
		if (this->_string[i] == '\\')
		{
			switch (this->_string[i + 1])
			{
				case 'n':
					processedValue += '\n';
					i++;
					break;
				case '\'':
					processedValue += '\'';
					i++;
					break;
				case 't':
					processedValue += '\t';
					i++;
					break;
				default:
					processedValue += this->_string[i];
					break;
			}
		} else
		{
			processedValue += this->_string[i];
		}
	}
    return processedValue; 
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::PlusPlusNode::getType(){
	return NUMBER;
}


void lp::PlusPlusNode::printAST() 
{
//   std::cout << "PlusPlusNode: ++"  << std::endl;
//   std::cout << "\t" << this->_id << std::endl;
}

double lp::PlusPlusNode::evaluateNumber()
{
	lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

	// Ckeck the type of the expression
	if (n->getType() == NUMBER)
	{
		n->setValue(n->getValue() + 1);
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for ","PlusPlus");
	}

  return n->getValue();
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::MinusMinusNode::getType(){
	return NUMBER;
}

void lp::MinusMinusNode::printAST() 
{
  std::cout << "MinusMinusNode: --"  << std::endl;
  std::cout << "\t" << this->_id << std::endl;
}

double lp::MinusMinusNode::evaluateNumber()
{
	lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

	// Ckeck the type of the expression
	if (n->getType() == NUMBER)
	{
		n->setValue(n->getValue() - 1);
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for ","MinusMinus");
	}

  return n->getValue();
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::NumericUnaryOperatorNode::getType()
{
	int result;

	if(this->_exp->getType() == NUMBER)
	{
		result = NUMBER;
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos incompatibles para ", "operador unario numerico");
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::LogicalUnaryOperatorNode::getType()
{
	int result;

	if(this->_exp->getType() == BOOL)
	{
		result = BOOL;
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos incompatibles para ", "operador unario logico");
	}
	
	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::NumericOperatorNode::getType()
{
	int result = 0;
		
	if ( (this->_left->getType() == NUMBER) and (this->_right->getType() == NUMBER))
		result = NUMBER;
	else
		warning("Error en tiempo de ejecucion: tipos incompatibles para ", "operador numerico");

	return	result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::StringOperatorNode::getType()
{
	int result = 0;
	
	if ((this->_left->getType() == STRING || this->_right->getType() == NUMBER) and 
		(this->_left->getType() == NUMBER || this->_right->getType() == STRING))
		result = STRING;
	else
		warning("Error en tiempo de ejecucion: tipos incompatibles para ", "operador de cadena");

	return	result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


int lp::RelationalOperatorNode::getType()
{
	int result = 0;
		
	if ( (this->_left->getType() == NUMBER) and (this->_right->getType() == NUMBER))
		result = BOOL;
	else if ( (this->_left->getType() == BOOL) and (this->_right->getType() == BOOL))
		result = BOOL;
	else if ( (this->_left->getType() == STRING) and (this->_right->getType() == STRING))
		result = BOOL;
	else
		warning("Error en tiempo de ejecucion: tipos incompatibles para ", "operador relacional");

	return	result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::LogicalOperatorNode:: getType()
{
	int result = 0;
		
	if ( (this->_left->getType() == BOOL) and (this->_right->getType() == BOOL))
	{
		// 
		result = BOOL;
	}
	else
		warning("Error en tiempo de ejecucion: tipos incompatibles para ", "operador logico");

	return	result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


void lp::UnaryMinusNode::printAST() 
{
//   std::cout << "UnaryMinusNode: -"  << std::endl;
//   std::cout << "\t"; 
//   this->_exp->printAST();
}

double lp::UnaryMinusNode::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the type of the expression
	if (this->getType() == NUMBER)
	{
		// Minus
		result = - this->_exp->evaluateNumber();
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos incompatibles para ", "resta unaria");
	}

  return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


void lp::UnaryPlusNode::printAST() 
{
//   std::cout << "UnaryPlusNode: +"  << std::endl;
//   std::cout << "\t"; 
//   this->_exp->printAST();
}

double lp::UnaryPlusNode::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the type of the expression
	if (this->getType() == NUMBER)
	{
		result = this->_exp->evaluateNumber();
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos incompatibles para ","suma unaria");
	}

  return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::PlusNode::printAST() 
{
	// std::cout << "PlusNode: +"  << std::endl;
	// std::cout << "\t"; 
	// this->_left->printAST();
	// std::cout << "\t"; 
	// this->_right->printAST();
}

double lp::PlusNode::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		result = this->_left->evaluateNumber() + this->_right->evaluateNumber();
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos incompatibles para ", "suma");
	}

  return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::MinusNode::printAST() 
{
//   std::cout << "MinusNode: -"  << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

double lp::MinusNode::evaluateNumber() 
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		result = this->_left->evaluateNumber() - this->_right->evaluateNumber();
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos incompatibles para ", "resta");
	}

  return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


void lp::MultiplicationNode::printAST() 
{
//   std::cout << "MultiplicationNode: *"  << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

double lp::MultiplicationNode::evaluateNumber() 
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		result = this->_left->evaluateNumber() * this->_right->evaluateNumber();
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos incompatibles para ","multiplicacion");
	}

  return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::DivisionNode::printAST()
{
//   std::cout << "DivisionNode: /" << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

double lp::DivisionNode::evaluateNumber() 
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		double leftNumber, rightNumber;

		leftNumber = this->_left->evaluateNumber();
		rightNumber = this->_right->evaluateNumber();
	
		// The divisor is not zero
    	if(std::abs(rightNumber) > ERROR_BOUND)
		{
				result = leftNumber / rightNumber;
		}
		else
		{
			warning("Error en tiempo de ejecucion", "division entre 0");
		}
	}
	else
	{
		warning("Error en tiempo de ejecucion: las expresiones no son numericas para ", "division");
	}

  return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void lp::IntDivisionNode::printAST()
{
//   std::cout << "IntDivisionNode: /" << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

double lp::IntDivisionNode::evaluateNumber() 
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		double leftNumber, rightNumber;

		leftNumber = this->_left->evaluateNumber();
		rightNumber = this->_right->evaluateNumber();
	
		// The divisor is not zero
    	if(std::abs(rightNumber) > ERROR_BOUND)
		{
				result = (int) leftNumber / (int) rightNumber;
		}
		else
		{
			warning("Error en tiempo de ejecucion", "division entre 0");
		}
	}
	else
	{
		warning("Error en tiempo de ejecucion: las expresiones no son numericas para ", "division entera");
	}

  return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ModuloNode::printAST()
{
//   std::cout << "ModuloNode: %" << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

double lp::ModuloNode::evaluateNumber() 
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		double leftNumber, rightNumber;

		leftNumber = this->_left->evaluateNumber();
		rightNumber = this->_right->evaluateNumber();
	
    	if(std::abs(rightNumber) > ERROR_BOUND)
				result = (int) leftNumber % (int) rightNumber;
		else
			warning("Error en tiempo de ejecucion", "division entre 0");
	}
	else
	{
		warning("Error en tiempo de ejecucion: las expresiones no son numericas para ", "modulo");
	}

  return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::PowerNode::printAST() 
{
//   std::cout << "PowerNode: ^"  << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

double lp::PowerNode::evaluateNumber() 
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		result = pow(this->_left->evaluateNumber(), this->_right->evaluateNumber());
	}
	else
	{
		warning("Error en tiempo de ejecucion: las expresiones no son numericas para ", "potencia");
	}

  return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ConcatNode::printAST() 
{
//   std::cout << "ConcatNode: ||"  << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

std::string lp::ConcatNode::evaluateString()
{
	std::string result = "";

	// Ckeck the types of the expressions
	if (this->getType() != STRING)
		warning("Error en tiempo de ejecucion: las expresiones no son cadenas para ", "concatenacion");

	result = this->_left->evaluateString() + this->_right->evaluateString();

  return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int lp::BuiltinFunctionNode_0::getType()
{
	return	NUMBER;
}


void lp::BuiltinFunctionNode_0::printAST() 
{
//   std::cout << "BuiltinFunctionNode_0: "  << std::endl;
//   std::cout << "\t";
//   std::cout << this->_id << std::endl;
}

double lp::BuiltinFunctionNode_0::evaluateNumber() 
{
	// Get the identifier in the table of symbols as BuiltinParameter0
	lp::BuiltinParameter0 *f = (lp::BuiltinParameter0 *) table.getSymbol(this->_id);

	// Apply the function and copy the result
   	return f->getFunction()();
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::BuiltinFunctionNode_1::getType()
{
	int result = 0;
		
	if (this->_exp->getType() == NUMBER)
		result = NUMBER;
	else
		warning("Runtime error: incompatible type for", "BuiltinFunctionNode_1");

	return	result;
}

void lp::BuiltinFunctionNode_1::printAST() 
{
//   std::cout << "BuiltinFunctionNode_1: "  << std::endl;
//   std::cout << "\t";
//   std::cout << this->_id << std::endl;  
//   std::cout << "\t";
// 	this->_exp->printAST();
// 	std::cout << std::endl;
}

double lp::BuiltinFunctionNode_1::evaluateNumber() 
{
	double result = 0.0;

	// Ckeck the type of the expression
	if (this->getType() == NUMBER)
	{
		// Get the identifier in the table of symbols as BuiltinParameter1
		lp::BuiltinParameter1 *f = (lp::BuiltinParameter1 *) table.getSymbol( this->_id);

		// Apply the function to the parameter and copy the result
		result = f->getFunction()(this->_exp->evaluateNumber());
	}
	else
	{
		warning("Runtime error: incompatible type of parameter for ", this->_id);
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::BuiltinFunctionNode_2::getType()
{
	int result = 0;
		
	if (this->_exp1->getType() == this->_exp2->getType())
		result = this->_exp1->getType();
	else
		warning("Error en tiemo de ejecucion: tipos incompatibles para", "BuiltinFunctionNode_2");

	return	result;
}


void lp::BuiltinFunctionNode_2::printAST() 
{
//   std::cout << "BuiltinFunctionNode_2: " << std::endl;
//   std::cout << "\t";
//   std::cout << this->_id << std::endl;
//   std::cout << "\t";
//   this->_exp1->printAST();
//   std::cout << "\t";
// 	this->_exp2->printAST();
// 	std::cout << std::endl;
}

double lp::BuiltinFunctionNode_2::evaluateNumber() 
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		// Get the identifier in the table of symbols as BuiltinParameter2
		lp::BuiltinParameter2 *f = (lp::BuiltinParameter2 *) table.getSymbol(this->_id);

		// Apply the function to the parameters and copy the result
	  	result = f->getFunction()(this->_exp1->evaluateNumber(),this->_exp2->evaluateNumber());
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles ", this->_id);
	}

  return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::GreaterThanNode::printAST()
{
//   std::cout << "GreaterThanNode: >" << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

bool lp::GreaterThanNode::evaluateBool() 
{
	bool result = false;

	if (this->getType() == BOOL and this->_left->getType() == STRING and this->_right->getType() == STRING)
	{
		const char* left_str = this->_left->evaluateString().c_str();
		const char* right_str = this->_right->evaluateString().c_str();
		result = strcmp(left_str, right_str) > 0;
	} 
	else if (this->getType() == BOOL)
	{
		double leftNumber, rightNumber;
		leftNumber = this->_left->evaluateNumber();
		rightNumber = this->_right->evaluateNumber();
		result = (leftNumber > rightNumber);
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles ", "operador mayor que");
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::GreaterOrEqualNode::printAST()
{
//   std::cout << "GreaterOrEqualNode: >= " << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

bool lp::GreaterOrEqualNode::evaluateBool() 
{
	bool result = false;

	if (this->getType() == BOOL and this->_left->getType() == STRING and this->_right->getType() == STRING)
	{
		const char* left_str = this->_left->evaluateString().c_str();
		const char* right_str = this->_right->evaluateString().c_str();
		result = strcmp(left_str, right_str) >= 0;
	} 
	else if (this->getType() == BOOL)
	{
		double leftNumber, rightNumber;
		leftNumber = this->_left->evaluateNumber();
		rightNumber = this->_right->evaluateNumber();

		result = (leftNumber >= rightNumber);
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles ", "operador mayor o igual que");
	}

	return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::LessThanNode::printAST()
{
//   std::cout << "LessThanNode: <" << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

bool lp::LessThanNode::evaluateBool() 
{
	bool result = false;

	if (this->getType() == BOOL and this->_left->getType() == STRING and this->_right->getType() == STRING)
	{
		const char* left_str = this->_left->evaluateString().c_str();
		const char* right_str = this->_right->evaluateString().c_str();
		result = strcmp(left_str, right_str) < 0;
	} 
	else if (this->getType() == BOOL)
	{
		double leftNumber, rightNumber;
		leftNumber = this->_left->evaluateNumber();
		rightNumber = this->_right->evaluateNumber();

		result = (leftNumber < rightNumber);
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles ", "operador menor que");
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::LessOrEqualNode::printAST()
{
//   std::cout << "LessOrEqualNode: <=" << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

bool lp::LessOrEqualNode::evaluateBool() 
{
	bool result = false;

	if (this->getType() == BOOL and this->_left->getType() == STRING and this->_right->getType() == STRING)
	{
		const char* left_str = this->_left->evaluateString().c_str();
		const char* right_str = this->_right->evaluateString().c_str();
		result = strcmp(left_str, right_str) <= 0;
	} 
	else if (this->getType() == BOOL)
	{
		double leftNumber, rightNumber;
		leftNumber = this->_left->evaluateNumber();
		rightNumber = this->_right->evaluateNumber();

		result = (leftNumber <= rightNumber);
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles ", "operador menor o igual que");
	}

	return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::EqualNode::printAST()
{
//   std::cout << "EqualNode: ==" << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

bool lp::EqualNode::evaluateBool() 
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		switch(this->_left->getType()){
			case NUMBER:
				double leftNumber, rightNumber;
				leftNumber = this->_left->evaluateNumber();
				rightNumber = this->_right->evaluateNumber();

				// ERROR_BOUND to control the precision of real numbers
				result = ( std::abs(leftNumber - rightNumber) < ERROR_BOUND );
			break;
			case BOOL:
				bool leftBoolean, rightBoolean;
				leftBoolean = this->_left->evaluateBool();
				rightBoolean = this->_right->evaluateBool();

				// 
				result = (leftBoolean == rightBoolean);
			break;
			case STRING:
				result = strcmp(this->_left->evaluateString().c_str(), this->_right->evaluateString().c_str()) == 0;
				break;
			default:
				warning("Error en tiempo de ejecucion: tipos de parametros incompatibles ", 
								"operador igual");				
		}
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles ", 
						"operador igual");
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::NotEqualNode::printAST()
{
//   std::cout << "NotEqualNode: !=" << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

bool lp::NotEqualNode::evaluateBool() 
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		switch(this->_left->getType()){
			case NUMBER:
				double leftNumber, rightNumber;
				leftNumber = this->_left->evaluateNumber();
				rightNumber = this->_right->evaluateNumber();

				// ERROR_BOUND to control the precision of real numbers
				result = ( std::abs(leftNumber - rightNumber) >= ERROR_BOUND );
			break;
			case BOOL:
				bool leftBoolean, rightBoolean;
				leftBoolean = this->_left->evaluateBool();
				rightBoolean = this->_right->evaluateBool();

				// 
				result = (leftBoolean != rightBoolean);
			break;
			case STRING:
				result = strcmp(this->_left->evaluateString().c_str(), this->_right->evaluateString().c_str()) != 0;
			break;
			default:
				warning("Error en tiempo de ejecucion: tipos de parametros incompatibles ", 
								"operador distinto");				
		}
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles ", "operador distinto");
	}

	return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::AndNode::printAST()
{
//   std::cout << "AndNode: &&" << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

bool lp::AndNode::evaluateBool() 
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		bool leftBool, rightBool;

		leftBool = this->_left->evaluateBool();
		rightBool = this->_right->evaluateBool();

		result = leftBool and rightBool;
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles ", "operdor #y");
	}

	return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::OrNode::printAST()
{
//   std::cout << "OrNode: ||" << std::endl;
//   std::cout << "\t"; 
// 	this->_left->printAST();
// 	std::cout << "\t"; 
// 	this->_right->printAST();
}

bool lp::OrNode::evaluateBool() 
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		bool leftBool, rightBool;

		leftBool = this->_left->evaluateBool();
		rightBool = this->_right->evaluateBool();

		result = leftBool or rightBool;
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles ", "operdor #o");
	}

	return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::NotNode::printAST()
{
//   std::cout << "NotNode: !" << std::endl;
//   std::cout << "\t";
//   this->_exp->printAST();
}

bool lp::NotNode::evaluateBool() 
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		result = not this->_exp->evaluateBool();
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles para ", "operador #no");
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::TernaryNode::printAST()
{
	// std::cout << "TernaryNode: ?" << std::endl;
	// std::cout << "\t";
	// this->_cond->printAST();
	// std::cout << "\t";
	// this->_true_exp->printAST();
	// std::cout << "\t";
	// this->_false_exp->printAST();
}

double lp::TernaryNode::evaluateNumber() 
{
	double result = 0.0;

	if (this->getType() == NUMBER)
	{
		result = this->_cond->evaluateBool() ? this->_true_exp->evaluateNumber() : this->_false_exp->evaluateNumber();
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles para ", "operador ternaria");
	}

	return result;
}

std::string lp::TernaryNode::evaluateString() 
{
	std::string result = "";

	if (this->getType() == STRING)
	{
		result = this->_cond->evaluateBool() ? this->_true_exp->evaluateString() : this->_false_exp->evaluateString();
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles para ", "operador ternaria");
	}

	return result;
}

bool lp::TernaryNode::evaluateBool() 
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		result = this->_cond->evaluateBool() ? this->_true_exp->evaluateBool() : this->_false_exp->evaluateBool();
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos de parametros incompatibles para ", "operador ternaria");
	}

	return result;
}

int lp::TernaryNode::getType()
{
	// Determina el tipo de nodo basado en los tipos de _true_exp y _false_exp
	// Aquí asumimos que _true_exp y _false_exp tienen el mismo tipo
	int trueType = _true_exp->getType();
	int falseType = _false_exp->getType();
	if (trueType == falseType)
	{
		return trueType;
	}
	else
	{
		warning("Error en tiempo de ejecucion: tipos incompatibles en las expresiones para", "operador ternaria");
		return -1; // Tipo no válido o error
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::AssignmentStmt::printAST() 
{
//   std::cout << "assignment_node: ="  << std::endl;
//   std::cout << "\t";
//   std::cout << this->_id << std::endl;
//   std::cout << "\t";

//   // Check the expression
// 	if (this->_exp != NULL)
// 	{
// 		this->_exp->printAST();
//     	std::cout << std::endl;
//   	}
//   // this->_asgn is not NULL: multiple assingment
//   else 
//     this->_asgn->printAST();

}

void lp::AssignmentStmt::evaluate() 
{
	/* Get the identifier in the table of symbols as Variable */
	/* 
		a = 2;
		a = b = 2;

		a: firstVar
		b: secondVar
	*/
	lp::Variable *firstVar = (lp::Variable *) table.getSymbol(this->_id);

	// Check the expression
	if (this->_exp != NULL)
	{
		// Check the type of the expression of the asgn
		switch(this->_exp->getType())
		{
			case NUMBER:
			{
				double value;
				// evaluate the expression as NUMBER
			 	value = this->_exp->evaluateNumber();

				// Check the type of the first varible
				if (firstVar->getType() == NUMBER)
				{
				  	// Get the identifier in the table of symbols as NumericVariable
					lp::NumericVariable *v = (lp::NumericVariable *) table.getSymbol(this->_id);

					// Assignment the value to the identifier in the table of symbols
					v->setValue(value);
				}
				// The type of variable is not NUMBER
				else
				{
					// Delete the variable from the table of symbols 
					table.eraseSymbol(this->_id);

					// Insert the variable in the table of symbols as NumericVariable 
					// with the type NUMBER and the value 
					lp::NumericVariable *v = new lp::NumericVariable(this->_id,
											VARIABLE,NUMBER,value);
					table.installSymbol(v);
				}
			}
			break;

			case BOOL:
			{
				bool value;
				// evaluate the expression as BOOL
			 	value = this->_exp->evaluateBool();

				if (firstVar->getType() == BOOL)
				{
				  	// Get the identifier in the table of symbols as LogicalVariable
					lp::LogicalVariable *v = (lp::LogicalVariable *) table.getSymbol(this->_id);

					// Assignment the value to the identifier in the table of symbols
					v->setValue(value);
				}
				// The type of variable is not BOOL
				else
				{
					// Delete the variable from the table of symbols 
					table.eraseSymbol(this->_id);

					// Insert the variable in the table of symbols as NumericVariable 
					// with the type BOOL and the value 
					lp::LogicalVariable *v = new lp::LogicalVariable(this->_id,
											VARIABLE,BOOL,value);
					table.installSymbol(v);
				}
			}
			break;

			case STRING:
			{
				std::string value;
				// evaluate the expression as STRING
			 	value = this->_exp->evaluateString();

				if (firstVar->getType() == STRING)
				{
				  	// Get the identifier in the table of symbols as StringVariable
					lp::StringVariable *v = (lp::StringVariable *) table.getSymbol(this->_id);
					// Assignment the value to the identifier in the table of symbols
					v->setValue(value);
				}
				// The type of variable is not STRING
				else
				{
					// Delete the variable from the table of symbols 
					table.eraseSymbol(this->_id);

					// Insert the variable in the table of symbols as NumericVariable 
					// with the type STRING and the value 
					lp::StringVariable *v = new lp::StringVariable(this->_id,
											VARIABLE,STRING,value);
					table.installSymbol(v);
				}
			}
			break;

			default:
				warning("Error en tiempo de ejecucion: tipo de expresion incompatible para ", "Asignacion");
		}

	}

	//////////////////////////////////////////////
	// Allow multiple assigment -> a = b = c = 2;

	else // this->_asgn is not NULL
	{
		// IMPORTANT
		//  evaluate the assigment child
		this->_asgn->evaluate();


		/* Get the identifier of the previous asgn in the table of symbols as Variable */
		lp::Variable *secondVar = (lp::Variable *) table.getSymbol(this->_asgn->_id);

		// Get the type of the variable of the previous asgn
		switch(secondVar->getType())
		{
			case NUMBER:
			{
				/* Get the identifier of the previous asgn in the table of symbols as NumericVariable */
				lp::NumericVariable *secondVar = (lp::NumericVariable *) table.getSymbol(this->_asgn->_id);
				// Check the type of the first variable
				if (firstVar->getType() == NUMBER)
				{
				/* Get the identifier of the first variable in the table of symbols as NumericVariable */
				lp::NumericVariable *firstVar = (lp::NumericVariable *) table.getSymbol(this->_id);
				  	// Get the identifier o f the in the table of symbols as NumericVariable
//					lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

					// Assignment the value of the second variable to the first variable
					firstVar->setValue(secondVar->getValue());

				}
				// The type of variable is not NUMBER
				else
				{
					// Delete the first variable from the table of symbols 
					table.eraseSymbol(this->_id);

					// Insert the first variable in the table of symbols as NumericVariable 
					// with the type NUMBER and the value of the previous variable 
					lp::NumericVariable *firstVar = new lp::NumericVariable(this->_id,
											VARIABLE,NUMBER,secondVar->getValue());
					table.installSymbol(firstVar);
				}
			}
			break;

			case BOOL:
			{
				/* Get the identifier of the previous asgn in the table of symbols as LogicalVariable */
				lp::LogicalVariable *secondVar = (lp::LogicalVariable *) table.getSymbol(this->_asgn->_id);
				// Check the type of the first variable
				if (firstVar->getType() == BOOL)
				{
				/* Get the identifier of the first variable in the table of symbols as LogicalVariable */
				lp::LogicalVariable *firstVar = (lp::LogicalVariable *) table.getSymbol(this->_id);
				  	// Get the identifier o f the in the table of symbols as NumericVariable
//					lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

					// Assignment the value of the second variable to the first variable
					firstVar->setValue(secondVar->getValue());

				}
				// The type of variable is not BOOL
				else
				{
					// Delete the first variable from the table of symbols 
					table.eraseSymbol(this->_id);

					// Insert the first variable in the table of symbols as NumericVariable 
					// with the type BOOL and the value of the previous variable 
					lp::LogicalVariable *firstVar = new lp::LogicalVariable(this->_id,
											VARIABLE,BOOL,secondVar->getValue());
					table.installSymbol(firstVar);
				}
			}
			break;

			case STRING:
			{
				/* Get the identifier of the previous asgn in the table of symbols as StringVariable */
				lp::StringVariable *secondVar = (lp::StringVariable *) table.getSymbol(this->_asgn->_id);
				// Check the type of the first variable
				if (firstVar->getType() == STRING)
				{
					/* Get the identifier of the first variable in the table of symbols as StringVariable */
					lp::StringVariable *firstVar = (lp::StringVariable *) table.getSymbol(this->_id);
				  	// Get the identifier o f the in the table of symbols as NumericVariable
					// lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

					// Assignment the value of the second variable to the first variable
					firstVar->setValue(secondVar->getValue());
				}
				// The type of variable is not STRING
				else
				{
					// Delete the first variable from the table of symbols 
					table.eraseSymbol(this->_id);

					// Insert the first variable in the table of symbols as NumericVariable 
					// with the type STRING and the value of the previous variable 
					lp::StringVariable *firstVar = new lp::StringVariable(this->_id,
											VARIABLE,STRING,secondVar->getValue());
					table.installSymbol(firstVar);
				}
			}
			break;

			default:
				warning("Error en tiempo de ejecucion: tipo de expresion incompatible para ", "Asignacion");
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::PlusAssignmentStmt::printAST() 
{
//   std::cout << "plus_assignment_node: +:="  << std::endl;
//   std::cout << "\t";
//   std::cout << this->_id << std::endl;
//   std::cout << "\t";

//   // Check the expression
// 	if (this->_exp != NULL)
// 	{
// 		this->_exp->printAST();
//     	std::cout << std::endl;
//   	}

}

void lp::PlusAssignmentStmt::evaluate() 
{
	/* Get the identifier in the table of symbols as Variable */
	/* 
		a += 2;

		a: firstVar
	*/
	lp::Variable *firstVar = (lp::Variable *) table.getSymbol(this->_id);

	// Check the expression
	if (this->_exp != NULL)
	{
		if(this->_exp->getType() == NUMBER)
		{
			double value;
			// evaluate the expression as NUMBER
			value = this->_exp->evaluateNumber();

			// Check the type of the first varible
			if (firstVar->getType() == NUMBER)
			{
				// Get the identifier in the table of symbols as NumericVariable
				lp::NumericVariable *v = (lp::NumericVariable *) table.getSymbol(this->_id);

				// Assignment the value to the identifier in the table of symbols
				v->setValue(value + v->getValue());
			}
			// The type of variable is not NUMBER
			else
			{
				// Delete the variable from the table of symbols 
				table.eraseSymbol(this->_id);

				// Insert the variable in the table of symbols as NumericVariable 
				// with the type NUMBER and the value 
				lp::NumericVariable *v = new lp::NumericVariable(this->_id,
										VARIABLE,NUMBER,value);
				table.installSymbol(v);
			}
		}
		else
		{
			warning("Runtime error: incompatible type of expression for ", "PlusAssigment");
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::MinusAssignmentStmt::printAST() 
{
//   std::cout << "plus_assignment_node: -:="  << std::endl;
//   std::cout << "\t";
//   std::cout << this->_id << std::endl;
//   std::cout << "\t";

//   // Check the expression
// 	if (this->_exp != NULL)
// 	{
// 		this->_exp->printAST();
//     	std::cout << std::endl;
//   	}

}

void lp::MinusAssignmentStmt::evaluate() 
{
	/* Get the identifier in the table of symbols as Variable */
	/* 
		a -= 2;

		a: firstVar
	*/
	lp::Variable *firstVar = (lp::Variable *) table.getSymbol(this->_id);

	// Check the expression
	if (this->_exp != NULL)
	{
		if(this->_exp->getType() == NUMBER)
		{
			double value;
			// evaluate the expression as NUMBER
			value = this->_exp->evaluateNumber();

			// Check the type of the first varible
			if (firstVar->getType() == NUMBER)
			{
				// Get the identifier in the table of symbols as NumericVariable
				lp::NumericVariable *v = (lp::NumericVariable *) table.getSymbol(this->_id);

				// Assignment the value to the identifier in the table of symbols
				v->setValue(value - v->getValue());
			}
			// The type of variable is not NUMBER
			else
			{
				// Delete the variable from the table of symbols 
				table.eraseSymbol(this->_id);

				// Insert the variable in the table of symbols as NumericVariable 
				// with the type NUMBER and the value 
				lp::NumericVariable *v = new lp::NumericVariable(this->_id,
										VARIABLE,NUMBER,value);
				table.installSymbol(v);
			}
		}
		else
		{
			warning("Runtime error: incompatible type of expression for ", "PlusAssigment");
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::PrintStmt::printAST() 
{
//   std::cout << "printASTStmt: printAST"  << std::endl;
//   std::cout << "\t";
//   this->_exp->printAST();
//   std::cout << std::endl;
}


void lp::PrintStmt::evaluate() 
{
	// std::cout << BIYELLOW; 
	// std::cout << "print: ";
	// std::cout << RESET; 

	switch(this->_exp->getType())
	{
		case NUMBER:
				std::cout << this->_exp->evaluateNumber();
				break;
		case BOOL:
			if (this->_exp->evaluateBool())
				std::cout << "verdadero";
			else
				std::cout << "falso";
		
			break;
		case STRING:
			std::cout << this->_exp->evaluateString();
			break;
		default:
			warning("Error en tiempo de ejecucion: tipo incompatible para ", "escribir()");
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ReadStmt::printAST() 
{
//   std::cout << "ReadStmt: read"  << std::endl;
//   std::cout << "\t";
//   std::cout << this->_id;
//   std::cout << std::endl;
}


void lp::ReadStmt::evaluate() 
{   
	double value;
	// std::cout << BIYELLOW; 
	// std::cout << "Insert a numeric value --> " ;
	// std::cout << RESET; 
	std::cin >> value;

	/* Get the identifier in the table of symbols as Variable */
	lp::Variable *var = (lp::Variable *) table.getSymbol(this->_id);

	// Check if the type of the variable is NUMBER
	if (var->getType() == NUMBER)
	{
		/* Get the identifier in the table of symbols as NumericVariable */
		lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);
						
		/* Assignment the read value to the identifier */
		n->setValue(value);
	}
	// The type of variable is not NUMBER
	else
	{
		// Delete $1 from the table of symbols as Variable
		table.eraseSymbol(this->_id);

			// Insert $1 in the table of symbols as NumericVariable 
		// with the type NUMBER and the read value 
		lp::NumericVariable *n = new lp::NumericVariable(this->_id, 
									  VARIABLE,NUMBER,value);

		table.installSymbol(n);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ReadStringStmt::printAST() 
{
//   std::cout << "ReadStringStmt: read"  << std::endl;
//   std::cout << "\t";
//   std::cout << this->_id;
//   std::cout << std::endl;
}


void lp::ReadStringStmt::evaluate() 
{   
	std::string value;
	// std::cout << BIYELLOW; 
	// std::cout << "Insert a alphanumeric value --> " ;
	// std::cout << RESET; 
	std::cin >> value;

	/* Get the identifier in the table of symbols as Variable */
	lp::Variable *var = (lp::Variable *) table.getSymbol(this->_id);

	// Check if the type of the variable is STRING
	if (var->getType() == STRING)
	{
		/* Get the identifier in the table of symbols as StringVariable */
		lp::StringVariable *n = (lp::StringVariable *) table.getSymbol(this->_id);
						
		/* Assignment the read value to the identifier */
		n->setValue(value);
	}
	// The type of variable is not STRING
	else
	{
		// Delete $1 from the table of symbols as Variable
		table.eraseSymbol(this->_id);

			// Insert $1 in the table of symbols as StringVariable 
		// with the type STRING and the read value 
		lp::StringVariable *n = new lp::StringVariable(this->_id, 
									  VARIABLE,STRING,value);

		table.installSymbol(n);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::EmptyStmt::printAST() 
{
//   std::cout << "EmptyStmt "  << std::endl;
}

void lp::EmptyStmt::evaluate() 
{
  // Empty
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// NEW in example 17

// TODO: ACA TRATAR STMT1 COMO UNA LISTA
void lp::IfStmt::printAST() 
{
//   std::cout << "IfStmt: "  << std::endl;
//   // Condition
//   std::cout << "\t";
//   this->_cond->printAST();

//   // Consequent
//   std::cout << "\t";
//   this->_stmt1->printAST();

//  // The alternative is printASTed if exists
//   if (this->_stmt2 != NULL)
//      {  
//        std::cout << "\t";
// 	   this->_stmt2->printAST();
//      }

//   std::cout << std::endl;
}


void lp::IfStmt::evaluate() 
{
   // If the condition is true,
	if (this->_cond->evaluateBool() == true ) {
     // the consequent is run 		
		this->_stmt1->evaluate();
	}
    // Otherwise, the alternative is run if exists
	else if (this->_stmt2 != NULL) {
		this->_stmt2->evaluate();
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// NEW in example 17

void lp::WhileStmt::printAST() 
{
//   std::cout << "WhileStmt: "  << std::endl;
//   // Condition
//   std::cout << "\t";
//   this->_cond->printAST();

//   // Body of the while loop
//   std::cout << "\t";
//   this->_stmt->printAST();

//   std::cout << std::endl;
}


void lp::WhileStmt::evaluate() 
{
  // While the condition is true. the body is run 
  while (this->_cond->evaluateBool() == true)
  {	
	  this->_stmt->evaluate();
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::RepeatStmt::printAST() 
{
	// std::cout << "RepeatStmt: "  << std::endl;
	// // Condition
	// std::cout << "\t";
	// this->_cond->printAST();

	// // Body of the repeat loop
	// std::cout << "\t";
	// this->_stmt->printAST();

	// std::cout << std::endl;
}

void lp::RepeatStmt::evaluate()
{
	do {
		this->_stmt->evaluate();
	} while(!this->_cond->evaluateBool());
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ForStmt::printAST()
{
	// std::cout << "ForStmt: "  << std::endl;
	// // Variable id
	// std::cout << "\t" << this->_id;

	// // Expression 1
	// std::cout << "\t";
	// this->_exp1->printAST();

	// // Expression 2
	// std::cout << "\t";
	// this->_exp2->printAST();

	// if (this->_exp3 != NULL)
	// {
	// 	std::cout << "\t";
	// 	this->_exp3->printAST();
	// }

	// // Body of the for loop
	// std::cout << "\t";
	// this->_stmt->printAST();

	// std::cout << std::endl;
}

void lp::ForStmt::evaluate()
{
	double value = this->_exp1->evaluateNumber();
	double end = this->_exp2->evaluateNumber();
	double inc = this->_exp3 != NULL ? this->_exp3->evaluateNumber() : 1;

	lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

	n->setValue(value);

	if(n->getType() == UNDEFINED)
		n->setType(NUMBER);

	if(value > end || inc < 0)
	{
		std::ostringstream oss;
		oss << "Valores incorrectos en bucle 'para' (inicio = " << value << ", final = " << end << ")\n";
		execerror("Error en tiempo de ejecucion: ", oss.str());
	}

	for(; n->getValue() <= end; n->setValue(n->getValue()+inc))
		this->_stmt->evaluate();
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::CaseStmt::printAST()
{
	// this->_expression->printAST();
	// std::cout << "\t";
	// this->_statements->printAST();
}

void lp::CaseStmt::evaluate()
{
	this->_statements->evaluate();
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::SwitchStmt::printAST()
{
	// std::list<CaseStmt *>::iterator it;

	// std::cout << "SwitchStmt: "  << std::endl;

	// for (it = this->_cases->begin(); it != this->_cases->end(); it++) 
	// {
	// 	(*it)->printAST();
	// }

	// if(this->_defaultStmt != NULL){
	// 	this->_defaultStmt->printAST();
	// }
	
}

void lp::SwitchStmt::evaluate()
{
	std::list<CaseStmt *>::iterator it;
	bool flag = false;

	for (it = this->_cases->begin(); it != this->_cases->end(); it++) 
	{
		switch((*it)->getExp()->getType())
		{
			case NUMBER:
				if((*it)->getExp()->evaluateNumber() == this->_exp->evaluateNumber())
				{
					(*it)->evaluate();
					flag = true;
				}
			break;

			case STRING:
				if((*it)->getExp()->evaluateString() == this->_exp->evaluateString())
				{
					(*it)->evaluate();
					flag = true;
				}
			break;

			case BOOL:
				if((*it)->getExp()->evaluateBool() == this->_exp->evaluateBool())
				{
					(*it)->evaluate();
					flag = true;
				}
			break;
		}
	}

	if(!flag && this->_defaultStmt != NULL){
		this->_defaultStmt->evaluate();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ClearStmt::printAST(){
	// std::cout << "ClearStmt";
}

void lp::ClearStmt::evaluate(){
	std::cout << CLEAR_SCREEN;
	PLACE(0,0);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::PlaceStmt::printAST(){
	// std::cout << "PlaceStmt";
}

void lp::PlaceStmt::evaluate(){
	PLACE((int)this->_exp1->evaluateNumber(), (int)this->_exp2->evaluateNumber());
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::DoWhileStmt::printAST() 
{
	// std::cout << "DoWhileStmt: "  << std::endl;
	// // Condition
	// std::cout << "\t";
	// this->_cond->printAST();

	// // Body of the repeat loop
	// std::cout << "\t";
	// this->_stmt->printAST();

	// std::cout << std::endl;
}

void lp::DoWhileStmt::evaluate()
{
	do {
		this->_stmt->evaluate();
	} while(this->_cond->evaluateBool());
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


void lp::PlusPlusStmt::printAST() 
{
//   std::cout << "PlusPlusStmt: ++"  << std::endl;
//   std::cout << "\t" << this->_id << std::endl << std::endl;
}

void lp::PlusPlusStmt::evaluate()
{
	lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

	// Ckeck the type of the expression
	if (n->getType() == NUMBER)
	{
		n->setValue(n->getValue() + 1);
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for ","PlusPlus");
	}

}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


void lp::MinusMinusStmt::printAST() 
{
//   std::cout << "MinusMinusStmt: ++"  << std::endl;
//   std::cout << "\t" << this->_id << std::endl << std::endl;
}

void lp::MinusMinusStmt::evaluate()
{
	lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

	// Ckeck the type of the expression
	if (n->getType() == NUMBER)
	{
		n->setValue(n->getValue() - 1);
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for ","PlusPlus");
	}

}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::BlockStmt::printAST() 
{
//   std::list<Statement *>::iterator stmtIter;

//   std::cout << "BlockStmt: "  << std::endl;

//   for (stmtIter = this->_stmts->begin(); stmtIter != this->_stmts->end(); stmtIter++) 
//   {
//      (*stmtIter)->printAST();
//   }
}


void lp::BlockStmt::evaluate() 
{
  std::list<Statement *>::iterator stmtIter;

  for (stmtIter = this->_stmts->begin(); stmtIter != this->_stmts->end(); stmtIter++) 
  {
    (*stmtIter)->evaluate();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::AST::printAST() 
{
//   std::list<Statement *>::iterator stmtIter;

//   for (stmtIter = stmts->begin(); stmtIter != stmts->end(); stmtIter++) 
//   {
//      (*stmtIter)->printAST();
//   }
}



void lp::AST::evaluate() 
{
  std::list<Statement *>::iterator stmtIter;

  for (stmtIter = stmts->begin(); stmtIter != stmts->end(); stmtIter++) 
  {
    (*stmtIter)->evaluate();
  }
}

