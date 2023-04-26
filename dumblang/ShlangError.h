#pragma once
#include <iostream>
#include <exception>
#include <map>
#include "AstNodes.h"
#include <string>

class LangError : public std::exception {
public:
	int line;
};
class UnspecifiedError : public LangError {
public:
	std::string errorMsg;
	virtual const char* what() {
		return errorMsg.data();
	};
	UnspecifiedError(std::string errorMsg_) {
		this->errorMsg = errorMsg_;
	}
};
class InterpreterError : public LangError {
};
class UndefinedVariableError : public InterpreterError {
public:
	std::string varName;
	virtual const char* what() {
		return ("Attempted to call Undefined Variable with name < "+varName+" >").data();
	};
	UndefinedVariableError(std::string varName_) {
		this->varName = varName_;
	};

};
class InvalidStringFormatError : public InterpreterError {
public:
	Value value;
	virtual const char* what() {
		return ("Invalid String Format specified format < " + std::get<std::string>(value) + " >").data();
	};
	InvalidStringFormatError(Value value_) {
		this->value = value_;
	};

};
class InvalidBranchFormatError : public InterpreterError {
public:
	virtual const char* what() {
		return "Invalid Branch Format";
	};
};
class InvalidOperatorError : public InterpreterError {
public:
	virtual const char* what() {
		return "Invalid Operator";
	};
};

class InvalidArgumentsError : public InterpreterError {
public:
	int lenght;
	int expectedLenght;
	virtual const char* what() {
		
		return ("Invalid Argument size expected: < " + std::to_string(expectedLenght) + " > got : <"+ std::to_string(lenght)+" >").data();
	};
	InvalidArgumentsError(int expectedLenght_ , int lenght_) {
		this->lenght = lenght_;
		this->expectedLenght = expectedLenght_;
	};

};
class InvalidTypeError : public InterpreterError {
public:
	Value specifiedType;
	Value exceptedType;
	const std::vector<std::string> typeMap = {"None","string","Function","FunctionCall","BuiltinFunc"};
	virtual const char* what() {
		return ("Invalid Type expected: < " + typeMap[exceptedType.index()] + " > got: <" + typeMap[specifiedType.index()] + " >").data();
	};
	InvalidTypeError(Value specifiedType_, Value exceptedType_) {
		this->specifiedType = specifiedType_;
		this->exceptedType = exceptedType_;
	};

};
class MixedTypesError : public InterpreterError {
public:
	std::vector<Value> specifiedType;
	std::vector<Value> exceptedType;
	const std::vector<std::string> typeMap = { "None","string","Function","FunctionCall","BuiltinFunc" };
	virtual const char* what() {
		return ("Mixed Types expected: < " + typeMap[exceptedType[0].index()] + "and: " + typeMap[exceptedType[1].index()] + " > got: <" + typeMap[specifiedType[0].index()] + "and: " + typeMap[specifiedType[1].index()] + " >").data();
	};
	MixedTypesError(std::vector<Value> specifiedType_, std::vector<Value> exceptedType_) {
		this->specifiedType = specifiedType_;
		this->exceptedType = exceptedType_;
	};

};
class InvalidCallError : public InterpreterError {
public:
	Value specifiedType;
	const std::vector<std::string> typeMap = { "None","string","Function","FunctionCall","BuiltinFunc" };
	virtual const char* what() {
		return ("Invalid Type expected: < Function > got: <" + typeMap[specifiedType.index()] + " >").data();
	};
	InvalidCallError(Value specifiedType_) {
		this->specifiedType = specifiedType_;
	};

};

