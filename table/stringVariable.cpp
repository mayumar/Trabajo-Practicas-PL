/*!	
	\file    numericVariable.cpp
	\brief   Code of some functions of NumericVariable class
	\author 
	\date    2017-10-19
	\version 1.0
*/


#include <iostream>

// Delete the comment if you want to use atof in the operator overload >>
// #include <stdlib.h>

#include "stringVariable.hpp"


/*
 Definitions of the read and write functions of the StringVariable class 
*/

void lp::StringVariable::read()
{
    // Inherited attributes
    std::cout << "Name of the StringVariable: ";
    std::cin >> this->_name;

    std::cout << "Token of the StringVariable: ";
    std::cin >> this->_token;
    // The \n character is read 
    std::cin.ignore(); 

    std::cout << "Type of the StringVariable: ";
    std::cin >> this->_type;
    // The \n character is read 
    std::cin.ignore(); 

    // Own attribute
    std::cout << "Value of the StringVariable: ";
    std::cin >> this->_value;
    // The \n character is read 
    std::cin.ignore(); 
}

void lp::StringVariable::write() const
{
    // Inherited methods
    std::cout << "Name:" << this->getName() << std::endl;
    std::cout << "Token:" << this->getToken() << std::endl;
    std::cout << "Type:" << this->getType() << std::endl;

    // Own method
    std::cout << "Value:" << this->getValue() << std::endl;
}

lp::StringVariable& lp::StringVariable::operator=(const lp::StringVariable &s)
{
// Check that is not the current object
    if (this != &s) 
    {
        // Inherited methods
        this->setName(s.getName());

        this->setToken(s.getToken());

        this->setType(s.getType());

        // Own method
        this->setValue(s.getValue());
    }

    // Return the current object
    return *this;
}