#include"Json.h"
#include <iostream>
#include <cassert>
class MyClass {
public:
    int id;
    std::string name;

    Json toJson() const {
        return Json::object{
            {"id", id},
            {"name", name}
        };
    }
};
void test1() {
    // 测试 nullptr 构造函数
    Json json_null = nullptr;
    assert(json_null.isNull());

    // 测试 double 构造函数
    Json json_double = 3.14;
    assert(json_double.isNumber() && std::abs(json_double.numberValue() - 3.14) < std::numeric_limits<double>::epsilon());

    // 测试 int 构造函数
    Json json_int = 42;
    assert(json_int.isNumber() && json_int.intValue() == 42);

    // 测试 bool 构造函数
    Json json_bool = true;
    assert(json_bool.isBoolean() && json_bool.boolValue() == true);

    // 测试 string 构造函数
    std::string str = "example";
    Json json_str = str;
    assert(json_str.isString() && json_str.stringValue() == "example");

    // 测试 array 构造函数
    Json::array arr = {json_int, json_bool};
    Json json_arr = arr;
    assert(json_arr.isArray() && json_arr.arrayItems().size() == 2);

    // 测试 object 构造函数
    Json::object obj = {{"key1", json_int}, {"key2", json_str}};
    Json json_obj = obj;
    assert(json_obj.isObject() && json_obj.objectItems().size() == 2);



    
    MyClass myObject{42, "Test Object"};
    Json json = myObject;

    // 使用 assert 验证转换结果
    assert(json.isObject());
    assert(json["id"].isNumber());
    assert(json["id"].intValue() == 42);
    assert(json["name"].isString());
    assert(json["name"].stringValue() == "Test Object");
    

}

void test2() {
    // 测试获取数字值
    Json json_number = 3.14;
    assert(std::abs(json_number.numberValue() - 3.14) < std::numeric_limits<double>::epsilon());

    // 测试获取整数值
    Json json_int = 42;
    assert(json_int.intValue() == 42);

    // 测试获取布尔值
    Json json_bool = true;
    assert(json_bool.boolValue() == true);

    // 测试获取字符串值
    Json json_str = "example";
    assert(json_str.stringValue() == "example");

    // 测试获取数组项
    Json::array arr = {json_int, json_bool};
    Json json_arr = arr;
    assert(json_arr.arrayItems().size() == 2 && json_arr[0].isNumber() && json_arr[1].isBoolean());

    // 测试获取对象项
    Json::object obj = {{"key", json_str}};
    Json json_obj = obj;
    assert(json_obj.objectItems().size() == 1 && json_obj["key"].isString());
}


void test3() {
    // 测试访问数组元素
    Json::array arr = {1, "two", false};
    Json json_arr = arr;
    assert(json_arr[0].intValue() == 1);
    assert(json_arr[1].stringValue() == "two");
    assert(json_arr[2].boolValue() == false);

    // 测试访问对象成员
    Json::object obj = {{"number", 1}, {"string", "two"}, {"boolean", false}};
    Json json_obj = obj;
    assert(json_obj["number"].intValue() == 1);
    assert(json_obj["string"].stringValue() == "two");
    assert(json_obj["boolean"].boolValue() == false);
}

void test_parse() {
    std::cout<<"============testing parse==========\n";
    std::string err;

    // 测试空对象
    auto json_empty_obj = Json::parse("{}", err);
    std::cout << "Empty Object: " << json_empty_obj.dump() << std::endl;

    // 测试简单数组
    auto json_array = Json::parse("[1, 2, 3, 4.5]", err);
    std::cout << "Simple Array: " << json_array.dump() << std::endl;

    // 测试嵌套数组
    auto json_nested_array = Json::parse("[1, [2, 3], 4]", err);
    std::cout << "Nested Array: " << json_nested_array.dump() << std::endl;

    // 测试简单对象
    auto json_simple_obj = Json::parse("{\"key1\": \"value1\", \"key2\": 42}", err);
    std::cout << "Simple Object: " << json_simple_obj.dump() << std::endl;

    // 测试嵌套对象
    auto json_nested_obj = Json::parse("{\"nested\": {\"key\": \"value\"}, \"array\": [1, 2, 3]}", err);
    std::cout << "Nested Object: " << json_nested_obj.dump() << std::endl;

    // 测试布尔值和null
    auto json_bool_null = Json::parse("{\"trueVal\": true, \"falseVal\": false, \"nullVal\": null}", err);
    std::cout << "Boolean and Null: " << json_bool_null.dump() << std::endl;

    // 测试字符串包含特殊字符
    auto json_special_chars = Json::parse("{\"path\": \"C:\\\\Program Files\\\\Example\"}", err);
    std::cout << "String with Special Characters: " << json_special_chars.dump() << std::endl;

    // 测试解析错误
    auto json_error = Json::parse("{\"incomplete: 1", err);
    if (!err.empty()) {
        std::cout << "Parsing Error: " << err << std::endl;
    } else {
        std::cout << "Parsing Error Test Failed (expected an error)" << std::endl;
    }
}


void test_dump() {
    std::cout<<"===========testing dump============\n";
    // 测试空对象
    Json json_empty_obj = Json::object{};
    std::cout << "Empty Object: " << json_empty_obj.dump() << std::endl;

    // 测试基本数据类型
    Json json_number = 42;
    std::cout << "Number: " << json_number.dump() << std::endl;

    Json json_str = "Hello, world!";
    std::cout << "String: " << json_str.dump() << std::endl;

    Json json_bool = true;
    std::cout << "Boolean: " << json_bool.dump() << std::endl;

    // 测试数组
    Json json_array = Json::array{1, "two", false, 3.14};
    std::cout << "Array: " << json_array.dump() << std::endl;

    // 测试对象
    Json json_object = Json::object{{"key1", "value1"}, {"key2", 42}, {"key3", true}};
    std::cout << "Object: " << json_object.dump() << std::endl;

    // 测试嵌套结构
    Json json_nested = Json::object{
        {"nested_array", Json::array{1, 2, 3}},
        {"nested_object", Json::object{{"nested_key", "nested_value"}}}
    };
    std::cout << "Nested Structures: " << json_nested.dump() << std::endl;
}




void test(){
    const Json obj = Json::object({
        { "k1", "v1" },
        { "k2", 42.0 },
        { "k3", Json::array({ "a", 123.0, true, false, nullptr }) },
    });

    std::cout << "obj: " << obj.dump() << "\n";


    const std::string simple_test =
        R"({"k1":"v1", "k2":42, "k3":["a",123,true,false,null]})";

    std::string err;
    const auto json = Json::parse(simple_test, err);

    std::cout << "k1: " << json["k1"].stringValue() << "\n";
    std::cout << "k3: " << json["k3"].dump() << "\n";

    for (auto &k : json["k3"].arrayItems()) {
        std::cout << "    - " << k.dump() << "\n";
    }
}
int main() {
    test1();
    test2();
    test3();

    std::cout << "All tests passed successfully." << std::endl;


    test_parse();
    test_dump();
    return 0;
}
