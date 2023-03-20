#include "Token.h"
#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>

std::string stringify(std::variant<double, std::string> const& value) {
	if (double const* pval = get_if<double>(&value))
		return std::to_string(*pval);
	return get<std::string>(value);
};
