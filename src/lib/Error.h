#ifndef ERROR_H
#define ERROR_H

#include <string>

using std::string;

namespace Packman {

class Error {
    public:
        Error(const string &error_string = "Undefined Error");
        Error(const string &error_string, const string &error_additional);
        string get_error_string();
    private:
        string error_text;
        string error_additional_text;
};

}

using namespace Packman;

#endif // ERROR_H