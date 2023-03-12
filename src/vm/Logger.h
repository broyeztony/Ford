//
//  Logger.h
//  Ford-VM
//
//  Created by Tony on 12/03/2023.
//

#ifndef Logger_h
#define Logger_h

#include <sstream>

class ErrorLogMessage : public std::basic_ostringstream<char> {
public:
    ~ErrorLogMessage() {
        fprintf(stderr, "Fatal error: %s\n", str().c_str());
        exit(EXIT_FAILURE);
    }
};

#define DIE ErrorLogMessage()

#endif
