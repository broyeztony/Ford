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
    BOOLEAN,
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
        bool boolean;
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

// constructeur
#define NUMBER(value) ((FordValue){FordValueType::NUMBER, .number = value})
#define BOOLEAN(value) ((FordValue){FordValueType::BOOLEAN, .boolean = value})
#define ALLOC_STRING(value) \
    ((FordValue){FordValueType::OBJECT, .object = (Object*)new StringObject(value)})
#define ALLOC_CODE(name) ((FordValue){FordValueType::OBJECT, .object = (Object*)new CodeObject(name)})

// converter
#define AS_NUMBER(fordValue) ((double)(fordValue).number)
#define AS_BOOLEAN(fordValue) ((bool)(fordValue).boolean)
#define AS_OBJECT(fordValue) ((Object*)(fordValue).object)
#define AS_STRING(fordValue) ((StringObject*)(fordValue).object)
#define AS_CPPSTRING(fordValue) (AS_STRING(fordValue)->string)
#define AS_CODE(fordValue) ((CodeObject*)(fordValue).object)


// type checking
#define IS_NUMBER(fordValue) ((fordValue).type == FordValueType::NUMBER)
#define IS_BOOLEAN(fordValue) ((fordValue).type == FordValueType::BOOLEAN)
#define IS_OBJECT(fordValue) ((fordValue).type == FordValueType::OBJECT)

#define IS_OBJECT_TYPE(fordValue, objectType) \
    (IS_OBJECT(fordValue) && AS_OBJECT(fordValue)->type == objectType)

#define IS_STRING(fordValue) IS_OBJECT_TYPE(fordValue, ObjectType::STRING)
#define IS_CODE(fordValue) IS_OBJECT_TYPE(fordValue, ObjectType::CODE)

std::string fordValueToTypeString(const FordValue& value) {
    if (IS_NUMBER(value)) {
        return "NUMBER";
    } else if (IS_BOOLEAN(value)) {
        return "BOOLEAN";
    } else if (IS_STRING(value)) {
        return "STRING";
    } else if (IS_CODE(value)) {
        return "CODE";
    } else {
        DIE << "fordValueToTypeString: unknown type " << (int)value.type;
    }
    return "";
}

std::string fordValueToConstantString(const FordValue& value) {
    std::stringstream ss;
    if (IS_NUMBER(value)) {
        ss << value.number;
    } else if (IS_BOOLEAN(value)) {
        ss << ((value.boolean == true) ? "true" : "false");
    } else if (IS_STRING(value)) {
        ss << '"' << AS_CPPSTRING(value) << '"';
    } else if (IS_CODE(value)) {
        auto code = AS_CODE(value);
        ss << "code " << code << ":" << code->name;
    } else {
      DIE << "fordValueToConstantString: unknown type " << (int)value.type;
    }
    return ss.str();
}

std::ostream& operator<<(std::ostream &os, const FordValue &value) {
    return os << "FordValue (" << fordValueToTypeString(value) << "): " << fordValueToConstantString(value);
}

#endif /* FordValue_h */
