
#include <iostream>
#include <vector>
#include <variant>
#include <map>
#include <stdexcept>
#include <exception>
#include <stack>
#include <string>
#include <optional>
#include "AstNodes.h"
#include "ShlangError.h"
bool Scope::isAtend() {
    return parentScope == nullptr;
}
Value Scope::getVar(std::string varName) {
    auto current = this;
    
    while (true) {
        
        if (current->varMap.contains(varName)) {

            return current->varMap[varName];
        }
        if (current->parentScope == nullptr) {
            break;
        }
        current = current->parentScope;
    }

    throw UndefinedVariableError(varName);
}
bool Scope::containsVar(std::string varName) {
    return varMap.contains(varName);
}
void Scope::define(std::string varName, Value val) {
    varMap.insert_or_assign(varName, val);
}