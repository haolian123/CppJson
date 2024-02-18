#include"Json.h"

static const int max_depth = 200;


struct NullStruct{
    bool operator==(NullStruct) const{ return true; }
    bool operator<(NullStruct) const { return false; }
};

static void dump(NullStruct, std::string &out) {
    out += "null";
}

static void dump(double value, std::string &out) {
    // 如果value是有限的
    if (std::isfinite(value)) {
        char buf[32];
        // 将value格式化为字符串，并存储在buf中
        snprintf(buf, sizeof buf, "%.17g", value);
        out += buf;
    } else {
        // 如果value不是有限的，将"null"追加到out中
        out += "null";
    }
}

// 定义一个静态函数dump，用于将整数value转化为字符串并追加到out中
static void dump(int value, std::string &out) {
    char buf[32];
    // 将整数value格式化为字符串，并存储在buf中
    snprintf(buf, sizeof buf, "%d", value);
    // 将buf中的内容追加到out中
    out += buf;
}

static void dump(bool value, std::string &out) {
    out += value ? "true" : "false";
}

static void dump(const std::string &value, std::string &out) {
    // 在输出字符串中添加双引号
    out += '"';
    // 遍历输入字符串的每个字符
    for (size_t i = 0; i < value.length(); i++) {
        const char ch = value[i];
        // 检查特殊字符并进行转义
        if (ch == '\\') {
            out += "\\\\"; // 转义反斜杠
        } else if (ch == '"') {
            out += "\\\""; // 转义双引号
        } else if (ch == '\b') {
            out += "\\b"; // 转义退格符
        } else if (ch == '\f') {
            out += "\\f"; // 转义换页符
        } else if (ch == '\n') {
            out += "\\n"; // 转义换行符
        } else if (ch == '\r') {
            out += "\\r"; // 转义回车符
        } else if (ch == '\t') {
            out += "\\t"; // 转义制表符
        } else if (static_cast<uint8_t>(ch) <= 0x1f) {
            // 对于ASCII控制字符，以Unicode码点形式进行转义
            char buf[8];
            snprintf(buf, sizeof buf, "\\u%04x", ch);
            out += buf;
        } else if (static_cast<uint8_t>(ch) == 0xe2 && static_cast<uint8_t>(value[i+1]) == 0x80
                   && static_cast<uint8_t>(value[i+2]) == 0xa8) {
            out += "\\u2028"; // 转义Unicode换行符U+2028
            i += 2;
        } else if (static_cast<uint8_t>(ch) == 0xe2 && static_cast<uint8_t>(value[i+1]) == 0x80
                   && static_cast<uint8_t>(value[i+2]) == 0xa9) {
            out += "\\u2029"; // 转义Unicode段落分隔符U+2029
            i += 2;
        } else {
            // 如果字符不需要转义，直接追加到out中
            out += ch;
        }
    }
    // 在输出字符串中添加双引号
    out += '"';
}

static void dump(const Json::array &values, std::string &out) {
    bool first = true; 
    out += "["; 
    for (const auto &value : values) {
        if (!first)
            out += ", "; 
        value.dump(out); 
        first = false; 
    }
    out += "]"; 
}

static void dump(const Json::object &values, std::string &out) {
    bool first = true; 
    out += "{"; 
    for (const auto &kv : values) {
        if (!first)
            out += ", "; 
        dump(kv.first, out); 
        out += ": "; 
        kv.second.dump(out); 
        first = false; 
    }
    out += "}"; 
}

// 定义Json类的成员函数dump，用于将Json对象转化为JSON字符串并追加到out中
void Json::dump(std::string &out) const {
    jsonValue->dump(out); // 调用JsonImpl类的dump函数将Json对象转化为JSON字符串并追加到out中
}

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

struct Statics{
    const std::shared_ptr<JsonValue> null = std::make_shared<JsonNull>();

    const std::shared_ptr<JsonValue> t = std::make_shared<JsonBoolean>(true);

    const std::shared_ptr<JsonValue> f = std::make_shared<JsonBoolean>(false);

    const std::string emptyString;

    const std::vector<Json> emptyVector;

    const std::map<std::string,Json> emptyMap;

    Statics(){}
};

static const Statics& statics(){
    static const Statics s{};
    return s;
}

static const Json & staticNull(){
    static const Json jsonNull;
    return jsonNull;
}


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

static inline std::string esc(char c) {
    char buf[12];
    if (static_cast<uint8_t>(c) >= 0x20 && static_cast<uint8_t>(c) <= 0x7f) {
        snprintf(buf, sizeof buf, "'%c' (%d)", c, c);
    } else {
        snprintf(buf, sizeof buf, "(%d)", c);
    }
    return std::string(buf);
}

static inline bool in_range(long x, long lower, long upper) {
    return (x >= lower && x <= upper);
}
