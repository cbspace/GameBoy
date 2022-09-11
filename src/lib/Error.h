#pragma once

#include <string>

using std::string;

class Error {
    public:
        Error(const string &error_string = "Undefined Error");
        Error(const string &error_string, const string &error_additional);
        string get_error_string();
    private:
        string error_text;
        string error_additional_text;
};