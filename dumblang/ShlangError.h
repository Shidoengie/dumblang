#pragma once
#include <iostream>
#include <exception>
#include <map>
#include "AstNodes.h"
#include <string>

class LangError : public std::exception {

public:
	std::string msg;
	int line = 0;
};
class UnspecifiedError : public LangError {
public:

	std::string errorMsg;
	virtual const char* what() const noexcept override {
		return errorMsg.c_str();
	};
	UnspecifiedError(std::string errorMsg_) {
		this->errorMsg = errorMsg_;
	}
};
class LexerError : public LangError {

};
class UndeterminedStringError : public LexerError {
public:
	virtual const char* what() const noexcept override {
		return "Undetermined string";
	};
};
class UnexpectedCharError : public LangError {
public:
	UnexpectedCharError(char8_t specifiedChar) {
		msg = "Unexpeted char, char:"+specifiedChar;
	}
	virtual const char* what() const noexcept override {
		return msg.c_str();
	};
};
class InterpreterError : public LangError {
	
};
class UndefinedVariableError : public InterpreterError {

public:
	std::string varName;
	virtual const char* what() const noexcept override {
		
		return msg.c_str();
	};
	UndefinedVariableError(std::string varName_) {
		this->varName = varName_;
		msg = ("Attempted to get the value of an Undefined Variable with name < " + varName + " >");
	};

};
class InvalidStringFormatError : public InterpreterError {
public:
	Value value;
	virtual const char* what() const noexcept {
		
		return msg.c_str();
	};
	InvalidStringFormatError(Value value_) {
		this->value = value_;
		msg = ("Invalid String Format specified format < " + std::get<std::string>(value) + " >");
	};

};
class InvalidBranchFormatError : public InterpreterError {
public:
	virtual const char* what() const noexcept override {
		return "Invalid Branch Format";
	};
};
class InvalidOperatorError : public InterpreterError {
public:
	virtual const char* what() const noexcept {
		return "Invalid Operator";
	};
};

class InvalidArgumentsError : public InterpreterError {

public:
	
	int lenght;
	int expectedLenght;
	virtual const char* what() const noexcept override {
		
		
		return msg.c_str();
	};
	InvalidArgumentsError(int expectedLenght_ , int lenght_) {
		this->lenght = lenght_;
		this->expectedLenght = expectedLenght_;
		msg = "Invalid Argument size expected : <" + std::to_string(expectedLenght) + " > got : <" + std::to_string(lenght) + " >";
	};

};
class InvalidTypeError : public InterpreterError {
public:
	Value specifiedType;
	Value exceptedType;
	const std::vector<std::string> typeMap = {"None","double","string","Function","FunctionCall","BuiltinFunc"};
	virtual const char* what() const noexcept override {
		return msg.c_str();
	};
	InvalidTypeError(Value specifiedType_, Value exceptedType_) {
		this->specifiedType = specifiedType_;
		this->exceptedType = exceptedType_;
		msg = ("Invalid Type expected: < " + typeMap[exceptedType.index()] + " > got: <" + typeMap[specifiedType.index()] + " >");
	};

};
class MixedTypesError : public InterpreterError {
public:
	std::vector<Value> specifiedType;
	std::vector<Value> exceptedType;
	
	std::vector<std::string> typeMap = { "None","double","string","Function","FunctionCall","BuiltinFunc"};
	virtual const char* what() const noexcept override {
		
		return msg.c_str();
	};
	MixedTypesError(std::vector<Value> specifiedType_, std::vector<Value> exceptedType_) {
		this->specifiedType = specifiedType_;
		this->exceptedType = exceptedType_;
		msg = ("Mixed Types expected: < " + typeMap[exceptedType[0].index()] + "and: " + typeMap[exceptedType[1].index()] + " > got: <" + typeMap[specifiedType[0].index()] + "and: " + typeMap[specifiedType[1].index()] + " >");
	};

};
class InvalidCallError : public InterpreterError {
public:
	Value specifiedType;
	std::vector<std::string> typeMap = { "None","double","string","Function","FunctionCall","BuiltinFunc"};
	virtual const char* what() const noexcept override {
		
		return msg.c_str();
	};
	InvalidCallError(Value specifiedType_) {
		this->specifiedType = specifiedType_;
		int indx = specifiedType.index();
		msg = ("Invalid Type expected: < Function > got: <" + typeMap[indx] + " >");
	};

};

