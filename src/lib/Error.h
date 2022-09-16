#pragma once

#include <string>

using std::string;

namespace CBLib {

class Error {
    public:
        Error(const string &error_string = "Undefined Error") {
            this->error_text = "Error: " + error_string;
        }

        Error(const string &error_string, const string &error_additional) {
            this->error_text = error_string;
            this->error_additional_text = error_additional;
        }

        string get_error_string() {
            if (error_additional_text.empty()) {
                return error_text;
            } else { 
                return error_text + " (" + error_additional_text + ")";
            }
        }
        
    private:
        string error_text;
        string error_additional_text;
};

}

using CBLib::Error;