//
//  FordCompiler.h
//  Ford-VM
//
//  Created by Tony on 20/03/2023.
//

#ifndef FordCompiler_h
#define FordCompiler_h

#include "../vm/FordValue.h"

using namespace std;

#define ALLOC_CONST(tester, converter, allocator, value)    \
    do {                                                    \
        for(auto i = 0 ; i < co->constants.size() ; i++) {  \
            if(!tester(co->constants[i])) {                 \
                continue;                                   \
            }                                               \
            else if(converter(co->constants[i]) == value) { \
                return i;                                   \
            }                                               \
        }                                                   \
        co->constants.push_back(allocator(value));          \
    } while(false)                                          \

#define GEN_BINARY_OP(op)                                   \
    do {                                                    \
        gen(exp.list[1]);                                   \
        gen(exp.list[2]);                                   \
        emit(op);                                           \
    } while (false)                                         \

class FordCompiler {
public:
    FordCompiler() {}

    CodeObject* compile(const Exp& exp) {

        FordValue codeValue = ALLOC_CODE("main");

        co = AS_CODE(codeValue);

        // recursively generates the bytecode
        gen(exp);

        emit(OP_HALT);

        return co;
    }

    void gen(const Exp& exp) {
        switch(exp.type) {

            case ExpType::NUMBER:
                emit(OP_CONST);
                emit(numericConstIdx(exp.number));
                break;

            case ExpType::STRING:
                emit(OP_CONST);
                emit(stringConstIdx(exp.string));
                break;

            case ExpType::SYMBOL:
                if (exp.string == "true" || exp.string == "false") {
                    emit(OP_CONST);
                    emit(booleanConstIdx(exp.string == "true" ? true : false));

                } else { // TODO: variables

                }
                break;

            case ExpType::LIST:
                auto tag = exp.list[0];

                if (tag.type == ExpType::SYMBOL) {
                    auto op = tag.string;
                    if (op == "+") {
                        GEN_BINARY_OP(OP_ADD);
                    } else if (op == "-") {
                        GEN_BINARY_OP(OP_SUB);
                    } else if (op == "*") {
                        GEN_BINARY_OP(OP_MUL);
                    } else if (op == "/") {
                        GEN_BINARY_OP(OP_DIV);
                    } else if (compareOps_.count(op) != 0) {
                        gen(exp.list[1]);
                        gen(exp.list[2]);
                        emit(OP_COMPARE);
                        emit(compareOps_[op]);
                    }
                }
                break;
        }
    }

private:

    size_t stringConstIdx(const std::string& value) {
        ALLOC_CONST(IS_STRING, AS_CPPSTRING, ALLOC_STRING, value);
        return co->constants.size() - 1;
    }

    size_t numericConstIdx(double value) {
        ALLOC_CONST(IS_NUMBER, AS_NUMBER, NUMBER, value);
        return co->constants.size() - 1;
    }

    size_t booleanConstIdx(bool value) {
        ALLOC_CONST(IS_BOOLEAN, AS_BOOLEAN, BOOLEAN, value);
        return co->constants.size() - 1;
    }

    void emit(uint8_t code) {
        co->code.push_back(code);
    }

    CodeObject* co;
    static map<string, uint8_t> compareOps_;
};

map<string, uint8_t> FordCompiler::compareOps_ {
    {"<", 0},{">", 1},{"<=", 2},{">=", 3},{"==", 4},{"!=", 5},
};

#endif /* FordCompiler_h */
