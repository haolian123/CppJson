#ifndef JSON_H
#define JSON_H 

#include<iostream>
#include<vector>
#include<map>
#include<initializer_list>
#include<memory>
#include<cmath>
#include<limits>
// 定义解析策略的枚举，支持标准解析和带注释的解析
enum JsonParse{
    STANDARD,COMMENTS
};

class JsonValue;

// Json 类定义
class Json{
public:
    // 定义 Json 支持的数据类型
    enum Type{
        NUL,NUMBER,BOOL,STRING,ARRAY,OBJECT
    };
    typedef std::vector<Json> array; // 定义数组类型
    typedef std::map<std::string,Json> object; // 定义对象类型
    // 构造函数
    Json() noexcept;
    Json(std::nullptr_t) noexcept;
    Json(double value);
    Json(int value);
    Json(bool value);
    Json(const std::string& value);
    Json(std::string&& value);
    Json(const char* value);
    Json(const array&value);
    Json(array&& values);
    Json(const object& values);
    Json(object && values);

    // 从具有 toJson 成员函数的类实例构造 Json
    template<class T,class = decltype(&T::toJson)>
    Json(const T & t) : Json(t.toJson()){}

    // 从支持 begin/end 迭代器的容器构造 Json 对象
    template <class M, typename std::enable_if<
        std::is_constructible<std::string, decltype(std::declval<M>().begin()->first)>::value
        && std::is_constructible<Json, decltype(std::declval<M>().begin()->second)>::value,
            int>::type = 0>
    Json(const M & m) : Json(object(m.begin(), m.end())) {}

    template <class V, typename std::enable_if<
        std::is_constructible<Json, decltype(*std::declval<V>().begin())>::value,
            int>::type = 0>
    Json(const V & v) : Json(array(v.begin(), v.end())) {}
    
    Json(void*) = delete; // 禁止从 void* 构造

    // 类型判断函数
    Type type() const;
    bool isNull() const{ return type()==NUL;}
    bool isNumber() const { return type()==NUMBER;}
    bool isBoolean() const { return type()==BOOL;}
    bool isString() const { return type()==STRING; }
    bool isArray() const { return type() == ARRAY; }
    bool isObject() const { return type() == OBJECT; }

    // 获取值的函数
    double numberValue() const;
    int intValue() const;
    bool boolValue() const;
    const std::string& stringValue() const;
    const array& arrayItems() const;
    const object &objectItems() const;

    // 重载 [] 操作符，用于访问数组元素和对象成员
    const Json & operator[] (size_t i) const;
    const Json & operator[] (const std::string &key) const;

    // 序列化函数
    void dump(std::string &out) const;
    std::string dump() const{
        std::string out;
        dump(out);
        return out;
    }

    // 解析 JSON 文本的静态函数
    static Json parse(const std::string&in, std::string& err, JsonParse strategy = JsonParse::STANDARD);
    static Json parse(const char* in, std::string& err, JsonParse strategy = JsonParse::STANDARD);

    // 解析多个 JSON 值的静态函数
    static std::vector<Json> parseMulti(
        const std::string&in,
        std::string::size_type & parserStopPos,
        std::string& err,
        JsonParse strateg = JsonParse::STANDARD
    );

    static std::vector<Json>parseMulti(
        const std::string & in,
        std::string & err,
        JsonParse strategy = JsonParse::STANDARD
    );

    // 重载比较运算符
    bool operator== (const Json &rhs) const;
    bool operator< (const Json &rhs) const;
    bool operator!= (const Json &rhs) const {return !(*this==rhs);}
    bool operator<= (const Json &rhs) const {return !(rhs<*this);}
    bool operator> (const Json &rhs) const { return (rhs<*this);}
    bool operator>= (const Json &rhs) const {return !(*this<rhs);}
    
    // 检查 Json 对象是否符合指定形状
    typedef std::initializer_list<std::pair<std::string,Type>> shape;
    bool hasShape(const shape &types,std::string &err) const;
    
private:
    std::shared_ptr<JsonValue> jsonValue; // 指向实际存储的智能指针
    
};



#endif
