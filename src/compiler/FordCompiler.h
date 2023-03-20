//
//  FordCompiler.h
//  Ford-VM
//
//  Created by Tony on 20/03/2023.
//

#ifndef FordCompiler_h
#define FordCompiler_h

#include "../vm/FordValue.h"

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
                break;
            
            case ExpType::LIST:
                break;
        }
    }

private:
    
    size_t stringConstIdx(const std::string& value) {
        for(auto i = 0 ; i < co->constants.size() ; i++) {
            if(!IS_STRING(co->constants[i])) {
                continue;
            }
            else if(AS_CPPSTRING(co->constants[i]) == value) {
                return i;
            }
        }
        co->constants.push_back(ALLOC_STRING(value));
        return co->constants.size() - 1;
    }
    
    size_t numericConstIdx(double value) {
        for(auto i = 0 ; i < co->constants.size() ; i++) {
            if(!IS_NUMBER(co->constants[i])) {
                continue;
            }
            else if(AS_NUMBER(co->constants[i]) == value) {
                return i;
            }
        }
        co->constants.push_back(NUMBER(value));
        return co->constants.size() - 1;
    }
    
    void emit(uint8_t code) {
        co->code.push_back(code);
    }
    
    CodeObject* co;
};

#endif /* FordCompiler_h */
