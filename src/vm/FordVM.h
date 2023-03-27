//
//  ForVM.h
//  Ford-VM
//
//  Created by Tony on 07/03/2023.
//

#ifndef ForVM_h
#define ForVM_h

#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <iomanip>
#include "OpCode.h"
#include "Logger.h"
#include "FordValue.h"
#include "../parser/FordParser.h"
#include "../compiler/FordCompiler.h"

using syntax::FordParser;

#define STACK_LIMIT     512
#define READ_BYTE()     *ip++
#define GET_CONST()     co->constants[READ_BYTE()]
#define BINARY_OP(operator)             \
do {                                    \
    auto op2 = AS_NUMBER(pop());        \
    auto op1 = AS_NUMBER(pop());        \
    push(NUMBER(op1 operator op2));     \
} while(false)

#define COMPARE_VALUES(op, v1, v2)      \
    do {                                \
        bool res;                       \
        switch (op) {                   \
            case 0:                     \
                res = v1 < v2;          \
                break;                  \
            case 1:                     \
                res = v1 > v2;          \
                break;                  \
            case 2:                     \
                res = v1 <= v2;         \
                break;                  \
            case 3:                     \
                res = v1 >= v2;         \
                break;                  \
            case 4:                     \
                res = v1 == v2;         \
                break;                  \
            case 5:                     \
                res = v1 != v2;         \
                break;                  \
        }                               \
        push(BOOLEAN(res));             \
    } while (false)                     \

class FordVM {

public:
    FordVM() : parser(std::make_unique<FordParser>()), compiler(std::make_unique<FordCompiler>()) {}

    void push(const FordValue& value) {
        if((size_t)(sp - stack.begin()) == STACK_LIMIT) {
            DIE << "push(): stack overflow.\n";
        }
        *sp = value;
        sp++;
    }

    FordValue pop() {
        if(sp == stack.begin()) {
            DIE << "pop(): empty stack.\n";
        }
        --sp;
        return *sp;
    }

    FordValue exec(const std::string &program) {

        // 1. parse AST
        auto ast = parser->parse(program);

        // 2. compile AST to bytecode
        co = compiler->compile(ast);

        // constants.push_back(NUMBER(10));
        // constants.push_back(NUMBER(3));
        // constants.push_back(NUMBER(10));
        // code = {OP_CONST, 0, OP_CONST, 1, OP_MUL, OP_CONST, 2, OP_SUB, OP_HALT};

        // constants.push_back(ALLOC_STRING("Hello, "));
        // constants.push_back(ALLOC_STRING("world!"));
        // code = {OP_CONST, 0, OP_CONST, 1, OP_ADD, OP_HALT};

        ip = &co->code[0];
        sp = &stack[0];

        return eval();
    }

    FordValue eval() {
        for(;;) {

            auto opcode = READ_BYTE();

            switch (opcode) {
                case OP_HALT:
                    return pop();
                case OP_CONST:
                    push(GET_CONST());
                    break;
                case OP_ADD: {
                    auto op2 = pop();
                    auto op1 = pop();

                    if(IS_NUMBER(op1) && IS_NUMBER(op2)){ // numeric addition
                        auto v1 = AS_NUMBER(op1);
                        auto v2 = AS_NUMBER(op2);
                        push(NUMBER(v1 + v2));
                    }
                    else if(IS_STRING(op1) && IS_STRING(op2)){ // string concatenation
                        auto s1 = AS_CPPSTRING(op1);
                        auto s2 = AS_CPPSTRING(op2);
                        push(ALLOC_STRING(s1 + s2));
                    }
                    break;
                }
                case OP_SUB: {
                    BINARY_OP(-);
                    break;
                }
                case OP_MUL: {
                    BINARY_OP(*);
                    break;
                }
                case OP_DIV: {
                    BINARY_OP(/);
                    break;
                }
                case OP_COMPARE: {
                    auto op = READ_BYTE();

                    auto op2 = pop();
                    auto op1 = pop();

                    if (IS_NUMBER(op1) && IS_NUMBER(op2)){ // numeric comparison
                        auto v1 = AS_NUMBER(op1);
                        auto v2 = AS_NUMBER(op2);

                        COMPARE_VALUES(op, v1, v2);
                    } else if (IS_STRING(op1) && IS_STRING(op2)){ // string comparison
                        auto s1 = AS_CPPSTRING(op1);
                        auto s2 = AS_CPPSTRING(op2);

                        COMPARE_VALUES(op, s1, s2);
                    }
                    break;
                }

                default:
                    DIE << "unknown opcode: " << std::hex << std::setfill('0') << std::setw(2) << unsigned(opcode);
            }
        }
    }

    std::unique_ptr<FordParser>         parser;
    std::unique_ptr<FordCompiler>       compiler;
    uint8_t*                            ip;
    FordValue*                          sp;

    CodeObject*                         co;
    std::array<FordValue, STACK_LIMIT>  stack;
};

#endif /* ForVM_h */
