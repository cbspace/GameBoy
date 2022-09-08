#include "Error.h"

Error::Error(const string &error_string) {
    this->error_text = "Error: " + error_string;
}

Error::Error(const string &error_string, const string &error_additional) {
    this->error_text = error_string;
    this->error_additional_text = error_additional;
}

string Error::get_error_string() {
    if (error_additional_text.empty()) {
        return error_text;
    } else { 
        return error_text + " (" + error_additional_text + ")";
    }
}