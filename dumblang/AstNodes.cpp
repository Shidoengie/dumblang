
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

    while (current != nullptr) {

        if (current->varMap.contains(varName)) {

            return current->varMap[varName];
        }
        current = current->parentScope;
    }

    throw UndefinedVariableError(varName);
}
bool Scope::containsVar(std::string varName) {
    return varMap.contains(varName);
}
void Scope::define(std::string varName, Value val) {
    auto current = this;
    current->varMap.insert_or_assign(varName, val);
}