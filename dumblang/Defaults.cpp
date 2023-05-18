#include "Defaults.h"
#include "ShlangError.h"
using std::cout;
using std::string;
Value PrintBuiltin(ValueStream arguments) {
	for (auto const& argument : arguments) {
		if (variantHas<std::string>(argument)) {
			cout << std::get<string>(argument) << '\n';
		}
		if (variantHas<double>(argument)) {
			cout << std::get<double>(argument) << '\n';
		}
		if (auto num = std::get_if<bool>(&argument)) {
			if (*num) {
				cout << "true" << "\n";
			}
			else {
				cout << "false" << "\n";
			}
		}
	}
	return NoneType();
}
Value InputBuiltin(ValueStream arguments) {
	PrintBuiltin(arguments);
	string out;
	std::getline(std::cin, out);
	return Value(out);
};
Value StringToNum(ValueStream arguments) {
	Value argument = arguments[0];
	if (auto val = std::get_if<string>(&argument)) {
		try {
			return std::stod(*val);
		}
		catch (std::invalid_argument) {
			throw InvalidStringFormatError(*val);
		}
	}
	if (variantHas<double>(argument)) {
		return argument;
	}

}
bool BoolConvert(double val) {
	bool out = (val != 0.0) ? true : false;
	return out;
};
std::optional<bool> ValToBool(Value val) {
	if (auto num = std::get_if<double>(&val)) {
		return BoolConvert(*num);
	}
	else if (auto num = std::get_if<bool>(&val)) {
		return *num;
	}
	else {
		return {};
	}
};
std::optional<double> ValToNum(Value val) {
	if (auto num = std::get_if<double>(&val)) {
		return *num;
	}
	else if(auto num = std::get_if<bool>(&val)) {
		return (double)*num;
	}
	else {
		return {};
	}
}
std::map<std::string, Value> getDefaultVarMap() {
	return {
		{"Test",2.0},
		{"PI",3.146210},
		{"str_num",BuiltinFunc(&StringToNum,1)},
		{"print",BuiltinFunc(&PrintBuiltin,-1)},
		{"input",BuiltinFunc(&InputBuiltin,1)}
	};
}
