/*!	
	\file   init.hpp
	\brief   Prototype of the function for the initialization of table of symbols
	\author  
	\date    2017-12-5
	\version 1.0
*/

#ifndef _INIT_HPP_
#define _INIT_HPP_

// sin, cos, atan, fabs, ...
#include <math.h>

#include "table.hpp"

// IMPORTANT: This file must be before y.tab.h
#include "../ast/ast.hpp"
///////////////////////////////////////

//  interpreter.tab.h contains the number values of the tokens produced by the parser
#include "../parser/interpreter.tab.h"

///////////////////////////////////////
#include "mathFunction.hpp"
#include "builtinParameter1.hpp"
///////////////////////////////////////

///////////////////////////////////////
#include "builtinParameter0.hpp"
#include "builtinParameter2.hpp"
///////////////////////////////////////

/*!
  \ brief Predefined numeric constants
*/
static struct {
          std::string name ;
	      double value;
	      } numericConstant[] = {
	                    {"pi",    3.14159265358979323846},
	                    {"e",     2.71828182845904523536},
	                    {"gamma", 0.57721566490153286060},
	                    {"deg",  57.29577951308232087680},
	                    {"phi",   1.61803398874989484820},
	                    {"",      0}
	                   };

/*!
  \ brief Predefined logical constants
*/
static struct {
          std::string name ;
	      bool value;
	      } logicalConstant[] = { 
	                    {"verdadero", true},
	                    {"falso", false},
	                    {"",      0}
	                   };


/*!
  \ brief Predefined keywords
*/
static struct {
          std::string name ;
	      int token;
	      } keyword[] = { 
	                    {"leer",  READ},
						{"leer_cadena", READ_STRING},
	                    {"escribir", PRINT},
						{"si", IF},
						{"entonces", THEN},
						{"si_no", ELSE},
						{"fin_si", END_IF},
						{"mientras", WHILE},
						{"hacer", DO},
						{"fin_mientras", END_WHILE},
						{"repetir", REPEAT},
						{"hasta", UNTIL},
						{"para", FOR},
						{"fin_para", END_FOR},
						{"desde", FROM},
						{"paso", STEP},
						{"casos", SWITCH},
						{"valor", CASE},
						{"defecto", DEFAULT},
						{"fin_casos", END_SWITCH},
						{"#o", OR},
						{"#y", AND},
						{"#no", NOT},
						{"borrar_pantalla", CLEAR},
						{"lugar", PLACE},
	                    {"",      0}
	                   };




static struct {    /* Predefined functions names */ 
                std::string name ;
				lp::TypePointerDoubleFunction_1 function;
              } function_1 [] = {
	                   {"sin",     sin},
		               {"cos",     cos},
		               {"atan",    atan},
		               {"log",     Log},
		               {"log10",   Log10},
		               {"exp",     Exp},
		               {"sqrt",    Sqrt},
		               {"integer", integer},
		               {"abs",     fabs},   
		               {"",       0}
		              };

static struct {   
                std::string name ;
				lp::TypePointerDoubleFunction_0 function;
              } function_0 [] = {
						{"random", Random},
		                {"",       0}
		              };



static struct {    /* Nombres predefinidos de funciones con 2 argumentos */ 
                std::string name ;
				lp::TypePointerDoubleFunction_2 function;
              } function_2 [] = {
	                   {"atan2",   Atan2},
		               {"",       0}
		              };







/*!		
	\brief   Initialize the table of symbols
	\return  void 
*/

void init(lp::Table &t);

// End of _INIT_HPP_
#endif
