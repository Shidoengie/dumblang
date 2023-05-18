#pragma once
#include <iostream>
#include <exception>
#include <vector>
#include <optional>
#include <sstream>
class InvalidRangeError : public std::exception {
public:

	std::string errorMsg;
	virtual const char* what() const noexcept override {
		return errorMsg.c_str();
	};
	InvalidRangeError(std::string errorMsg_) {
		this->errorMsg = errorMsg_;
	}
};
class Range {
public:
	size_t start;
	size_t stop;
	size_t max;
	Range(size_t max_);
	Range(size_t max_, size_t start_, size_t stop_);
    void setRange(size_t start_, size_t stop_);
	std::vector<size_t> AsVec();
	std::optional<std::string> splice(std::string source);
};


