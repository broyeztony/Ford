//
//  FordValue.h
//  Ford-VM
//
//  Created by Tony on 12/03/2023.
//

#ifndef FordValue_h
#define FordValue_h

#include <string>

enum class FordValueType {
    NUMBER,
    OBJECT,
};

enum class ObjectType {
    STRING,
    CODE,
};

struct Object {
    Object(ObjectType type) : type(type) {}
    ObjectType type;
};

struct StringObject : public Object {
    StringObject(const std::string &str)
    : Object(ObjectType::STRING), string(str) {}
    std::string string;
};

struct FordValue {
    FordValueType type;
    union {
        double  number;
        Object* object;
    };
};

struct CodeObject : public Object {
    CodeObject(const std::string& name)
    : Object(ObjectType::CODE), name(name) {}
    
    std::string                 name;
    std::vector<FordValue>      constants;
    std::vector<uint8_t>        code;
};

#define NUMBER(value) ((FordValue){FordValueType::NUMBER, .number = value})
#define AS_NUMBER(fordValue) ((double)(fordValue).number)
#define AS_OBJECT(fordValue) ((Object*)(fordValue).object)

#define ALLOC_STRING(value) \
    ((FordValue){FordValueType::OBJECT, .object = (Object*)new StringObject(value)})
#define AS_STRING(fordValue) ((StringObject*)(fordValue).object)
#define AS_CPPSTRING(fordValue) (AS_STRING(fordValue)->string)

#define ALLOC_CODE(name) ((FordValue){FordValueType::OBJECT, .object = (Object*)new CodeObject(name)})
#define AS_CODE(fordValue) ((CodeObject*)(fordValue).object)


// type checking
#define IS_NUMBER(fordValue) ((fordValue).type == FordValueType::NUMBER)
#define IS_OBJECT(fordValue) ((fordValue).type == FordValueType::OBJECT)

#define IS_OBJECT_TYPE(fordValue, objectType) \
    (IS_OBJECT(fordValue) && AS_OBJECT(fordValue)->type == objectType)

#define IS_STRING(fordValue) IS_OBJECT_TYPE(fordValue, ObjectType::STRING)
#define IS_CODE(fordValue) IS_OBJECT_TYPE(fordValue, ObjectType::CODE)

#endif /* FordValue_h */
