#pragma once
#include "ShlangError.h"
template<class Ty, class... Types>
constexpr bool variantHas(const std::variant<Types...>& var) noexcept {
	return std::holds_alternative<Ty>(var);
}

Value PrintBuiltin(NodeStream arguments);
Value InputBuiltin(NodeStream arguments);
Value StringToNum(NodeStream arguments);
bool BoolConvert(double val);
std::optional<bool> ValToBool(Value val);
std::optional<double> ValToNum(Value val);
std::map<std::string, Value> getDefaultVarMap();