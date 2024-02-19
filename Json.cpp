
#include"Global.h"

/*************构造函数******************/
Json::Json() noexcept : jsonValue(statics().null) {}

Json::Json(std::nullptr_t) noexcept : jsonValue(statics().null) {}

Json::Json(double value) : jsonValue(std::make_shared<JsonDouble>(value)) {}

Json::Json(int value) : jsonValue(std::make_shared<JsonInt>(value)) {}

Json::Json(bool value) : jsonValue(value ? statics().t : statics().f) {}

Json::Json(const std::string& value) : jsonValue(std::make_shared<JsonString>(value)) {}

Json::Json(std::string&& value) : jsonValue(std::make_shared<JsonString>(std::move(value))) {}

Json::Json(const char* value) : jsonValue(std::make_shared<JsonString>(value)) {}

Json::Json(const Json::array& value) : jsonValue(std::make_shared<JsonArray>(value)) {}

Json::Json(Json::array&& value) : jsonValue(std::make_shared<JsonArray>(std::move(value))) {}

Json::Json(const Json::object& value) : jsonValue(std::make_shared<JsonObject>(value)) {}

Json::Json(Json::object &&value) : jsonValue(std::make_shared<JsonObject>(std::move(value))) {}

/************* Member Functions ******************/

Json::Type Json::type() const {
    return jsonValue->type();
}

double Json::numberValue() const {
    return jsonValue->numberValue();
}

int Json::intValue() const {
    return jsonValue->intValue();
}

bool Json::boolValue() const {
    return jsonValue->boolValue();
}

const std::string & Json::stringValue() const {
    return jsonValue->stringValue();
}

const std::vector<Json> & Json::arrayItems() const {
    return jsonValue->arrayItems();
}

const std::map<std::string, Json> & Json::objectItems() const {
    return jsonValue->objectItems();
}

const Json & Json::operator[] (size_t i) const {
    return (*jsonValue)[i];
}

const Json & Json::operator[] (const std::string& key) const {
    return (*jsonValue)[key];
}

double JsonValue::numberValue() const {
    return 0;
}

int JsonValue::intValue() const {
    return 0;
}

bool JsonValue::boolValue() const {
    return false;
}

const std::string& JsonValue::stringValue() const {
    return statics().emptyString;
}

const std::vector<Json> & JsonValue::arrayItems() const {
    return statics().emptyVector;
}

const std::map<std::string, Json> & JsonValue::objectItems() const {
    return statics().emptyMap;
}

const Json& JsonValue::operator[] (size_t) const{
    return staticNull();
}

const Json& JsonValue::operator[] (const std::string&) const{
    return staticNull();
}

const Json& JsonObject::operator[] (const std::string&key) const{
    auto it = value.find(key);
    return (it==value.end()) ? staticNull() : it->second;
}

const Json& JsonArray::operator[](size_t i ) const{
    if(i>=value.size()) return staticNull();
    return value[i];
}

/*比较*/

bool Json::operator== (const Json&other) const{
    if (jsonValue == other.jsonValue)
        return true;
    if (jsonValue->type() != other.jsonValue->type())
        return false;
    return jsonValue->equals(other.jsonValue.get());
}

bool Json::operator< (const Json& other) const{
    if (jsonValue == other.jsonValue)
        return false;
    if (jsonValue->type() != other.jsonValue->type())
        return jsonValue->type() < other.jsonValue->type();
    return jsonValue->less(other.jsonValue.get());
}


// 定义Json类的成员函数dump，用于将Json对象转化为JSON字符串并追加到out中
void Json::dump(std::string &out) const {
    jsonValue->dump(out); // 调用JsonImpl类的dump函数将Json对象转化为JSON字符串并追加到out中
}

