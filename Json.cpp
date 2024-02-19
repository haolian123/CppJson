
#include"Global.h"
#include "Parse.h"
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



// 将字符串转化为Json对象
Json Json::parse(const std::string &in, std::string &err, JsonParse strategy) {
    // 初始化一个Json解析器
    JsonParser parser { in, 0, err, false, strategy };
    // 解析输入字符串以得到Json结果
    Json result = parser.parseJson(0);

    // 检查是否有尾随的垃圾字符
    parser.consumeGarbage();
    if (parser.failed)
        return Json(); // 如果解析失败，返回一个空的Json对象
    if (parser.i != in.size())
        return parser.fail("unexpected trailing " + esc(in[parser.i])); // 如果输入字符串尚有未解析内容，报告错误

    return result; // 返回解析得到的Json对象
}

Json Json::parse(const char* in, std::string& err, JsonParse strategy ){
    if (in) {
            return parse(std::string(in), err, strategy);
    } else {
        err = "null input";
        return nullptr;
    }
}
// 解析输入字符串中的多个Json对象
std::vector<Json> Json::parseMulti(const std::string &in,
                               std::string::size_type &parser_stop_pos,
                               std::string &err,
                               JsonParse strategy) {
    // 初始化一个Json解析器
    JsonParser parser { in, 0, err, false, strategy };
    parser_stop_pos = 0;
    std::vector<Json> jsonList; // 存储解析得到的多个Json对象的容器

    // 当输入字符串还有内容并且解析未出错时继续
    while (parser.i != in.size() && !parser.failed) {
        jsonList.push_back(parser.parseJson(0)); // 解析Json对象并添加到容器中
        if (parser.failed)
            break; // 如果解析失败，中断循环

        // 检查是否还有其他对象
        parser.consumeGarbage();
        if (parser.failed)
            break; // 如果发现垃圾字符或有错误，中断循环
        parser_stop_pos = parser.i; // 更新停止位置
    }
    return jsonList; // 返回解析得到的Json对象容器
}

std::vector<Json> Json::parseMulti(
        const std::string & in,
        std::string & err,
        JsonParse strategy
    )
{
        std::string::size_type parser_stop_pos;
        return parseMulti(in, parser_stop_pos, err, strategy);
}

// 检查 JSON 对象是否具有指定的形状
bool Json::hasShape(const shape & types, std::string & err) const {
    // 如果 JSON 不是对象类型，则返回错误
    if (!isObject()) {
        err = "expected JSON object, got " + dump();
        return false;
    }

    // 获取 JSON 对象的所有成员项
    const auto& obj_items = objectItems();
    
    // 遍历指定的形状
    for (auto & item : types) {
        // 查找 JSON 对象中是否存在形状中指定的项
        const auto it = obj_items.find(item.first);
        
        // 如果找不到项或者项的类型不符合指定的类型，则返回错误
        if (it == obj_items.cend() || it->second.type() != item.second) {
            err = "bad type for " + item.first + " in " + dump();
            return false;
        }
    }

    // 如果所有形状都匹配，则返回 true
    return true;
}