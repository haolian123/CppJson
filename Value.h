#ifndef VALUE_H
#define VALUE_H
#include"Json.h"
#include "Dump.h"
// Json 值的基类
class JsonValue{
protected:
    friend class Json;
    friend class JsonInt;
    friend class JsonDouble;
    virtual Json::Type type() const = 0;
    virtual bool equals(const JsonValue*other) const = 0;
    virtual bool less(const JsonValue*other) const = 0;
    virtual void dump(std::string& out) const = 0;
    virtual double numberValue() const;
    virtual int intValue() const;
    virtual bool boolValue() const;
    virtual const std::string &stringValue() const;
    virtual const Json::array &arrayItems() const;
    virtual const Json::object &objectItems() const;
    virtual const Json& operator[] (size_t i) const;
    virtual const Json& operator[](const std::string &key) const;
    virtual ~JsonValue(){}

};

template<Json::Type tag,typename T>
class Value : public JsonValue{
protected:
    const T value;
protected:
    explicit Value(const T& value) : value(value){}
    explicit Value(T&&value) : value(std::move(value)){}

    Json::Type type() const override{
        return tag;
    }

    bool equals(const JsonValue* other) const override {
        return value == static_cast<const Value<tag,T>*>(other)->value;
    }

    bool less(const JsonValue* other) const override{
        return value < static_cast<const Value<tag, T> *>(other)->value;
    }

    void dump(std::string&out) const override{ ::dump(value,out);}
};


class JsonDouble final : public Value<Json::NUMBER,double>{
    double numberValue() const override {return value;}
    int intValue() const override { return static_cast<int>(value);}
    bool equals(const JsonValue* other) const override{ return value==other->numberValue();}
    bool less (const JsonValue* other) const override {return value<other->numberValue();}
public:
    explicit JsonDouble(double value) : Value(value){}
};

class JsonInt final: public Value<Json::NUMBER,int>{
    double numberValue() const override { return value; }
    int intValue() const override { return value; }
    bool equals(const JsonValue* other) const override{ return value == other->numberValue();}
    bool less(const JsonValue* other) const override{ return value < other->numberValue();}
public:
    explicit JsonInt(int value): Value(value){}
};

class JsonBoolean final : public Value<Json::BOOL,bool>{
    bool boolValue() const override { return value; }
public:
    explicit JsonBoolean(bool value):Value(value){}
};

class JsonString final:public Value<Json::STRING,std::string>{
    const std::string & stringValue() const override { return value;}
public:
    explicit JsonString(const std::string& value): Value(value){}
    explicit JsonString(std::string&& value) : Value(std::move(value)) {}
};

class JsonArray final : public Value<Json::ARRAY, Json::array>{
    const Json::array & arrayItems() const override{ return value;}
    const Json & operator[](size_t i) const override;
public:
    explicit JsonArray(const Json::array &value): Value(value){}
    explicit JsonArray(Json::array &&value) : Value(std::move(value)){}
};

class JsonObject final : public Value<Json::OBJECT,Json::object>{
    const Json::object & objectItems() const override{ return value;}
    const Json& operator[] (const std::string& key) const override;
public:
    explicit JsonObject(const Json::object &value) : Value(value) {} 
    explicit JsonObject(Json::object &&value)      : Value(std::move(value)) {} 
};

class JsonNull final: public Value<Json::NUL,NullStruct>{
public:
    JsonNull() : Value({}) {}
};

#endif