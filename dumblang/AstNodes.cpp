
#include <iostream>
#include <vector>
#include <variant>
#include <map>
#include <stdexcept>
#include <exception>
#include <stack>
#include <string>
#include <optional>
#include "ShlangError.h"
#include "AstNodes.h"

bool Scope::isAtend() {
	return parentScope == nullptr;
}
Value Scope::getVar(std::string varName) {
    auto current = this;
    
    while (current != nullptr) {
        std::cout << varName << "\n";
        std::cout << current->varMap.contains(varName) << "\n";
        if (current->varMap.contains(varName)) { //`find` returns the end iterator if there is no such element.
            
            return current->varMap[varName];
        }
        current = current->parentScope;
    }

    throw LangError("Undeclared Variable", LangError::AST);
}
bool Scope::containsVar(std::string varName) {
	return varMap.contains(varName);
}
void Scope::define(std::string varName, Value val) {
    auto current = this;
	current->varMap.insert_or_assign(varName, val);
}
