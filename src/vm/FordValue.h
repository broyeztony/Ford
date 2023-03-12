//
//  FordValue.h
//  Ford-VM
//
//  Created by Tony on 12/03/2023.
//

#ifndef FordValue_h
#define FordValue_h

enum class FordValueType {
    NUMBER,
};

struct FordValue {
    FordValueType type;
    union {
        double number;
    };
};

#define NUMBER(value) ((FordValue){FordValueType::NUMBER, .number = value})

#endif /* FordValue_h */
