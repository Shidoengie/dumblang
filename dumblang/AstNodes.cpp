
#include <iostream>
#include <vector>
#include <variant>
#include <map>
#include <stdexcept>
#include <exception>
#include <stack>
#include <string>
#include "AstNodes.h"

bool Scope::isAtend() {
	return parentScope == nullptr;
}
Value Scope::getVar(std::string varName) {
	return varMap[varName];
}
bool Scope::containsVar(std::string varName) {
	return varMap.contains(varName);
}
void Scope::define(std::string varName, Value val) {
	varMap.insert_or_assign(varName, val);
}
