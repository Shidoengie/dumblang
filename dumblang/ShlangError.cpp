#include "ShlangError.h"


const std::string LangError::what() {
	return msg;
}
LangError::LangError(std::string message, Type errorType) {
	msg = message;
	type = errorType;
}
