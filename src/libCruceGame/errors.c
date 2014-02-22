#include "errors.h"

char *error(int error_code)
{
    switch(error_code) {

        case NO_ERROR:
            return "There is no error";
        break;

        default:
            return "There is no error code for this value";
    }
}
