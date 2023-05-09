
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

std::string ValueToString(Value val) {
    if (std::holds_alternative<NoneType>(val)) {
        return "None";
    }
    else if (auto obj = std::get_if<double>(&val)) {
        return std::to_string(*obj);
    }
    else if (auto obj = std::get_if<std::string>(&val)) {
        return *obj;
    }
    else if (std::holds_alternative<Function>(val)) {
        return "Function";
    }
    else if (std::holds_alternative<BuiltinFunc>(val)) {
        return "Builtin Function";
    }
}
bool Scope::isAtend() {
    return parentScope == nullptr;
}
std::optional<Value> Scope::getVar(std::string varName) {
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

    return {};
}
bool Scope::containsVar(std::string varName) {
    return varMap.contains(varName);
}
void Scope::define(std::string varName, Value val) {
    varMap.insert_or_assign(varName, val);
}
std::string Scope::to_string() {
    std::string out = "";
    for (auto& item : this->varMap) {
        out += "{" + item.first + " : " + ValueToString(item.second) + "} ";
    }
    return out;
}
std::string Scope::structure() {
    auto current = this;
    std::string scopeStruct;
    while (true) {

        if (current->parentScope == nullptr) {
            break;
        }
        std::string out = "";
        for (auto& item : current->varMap) {
            out += "{" + item.first + " : " + ValueToString(item.second) + "} ";
        }
        scopeStruct += "["+out;
        
        current = current->parentScope;
    }
    return scopeStruct;
}