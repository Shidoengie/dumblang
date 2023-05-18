#include "range.h"



Range::Range(size_t max_) {
    start = 0;
    stop = 0;
    max = max_;
}

Range::Range(size_t max_, size_t start_, size_t stop_) {
    
    if (start_ > stop_) {
        throw InvalidRangeError("start is greater then stop");
    }
    if (start_ > max_ || stop_ > max_) {
        std::ostringstream out;
        out << "range greater then max Range: " << "max:" << max_ << " start:" << start_ << " stop:" << stop_;
        throw InvalidRangeError(out.str());
    }
    max = max_;
    start = start_;
    stop = stop_;
}
std::string Range::to_string() {
    std::ostringstream out;
    out << "max:" << max << " start:"<<start<<" stop:"<<stop;
    return out.str();
}
void Range::setRange(size_t start_, size_t stop_) {
    if (start_ > stop_) {
        throw InvalidRangeError("start is greater then stop");
    }
    if (start_ > max || stop_ > max) {
        throw InvalidRangeError("range greater then max");
    }
    start = start_;
    stop = stop_;
}
std::vector<size_t> Range::AsVec() {
    return {start,stop};
}
std::optional<std::string> Range::splice(std::string source) {
    if (max > source.length()) {
        return {};
    }
    return source.substr(start,stop);
}