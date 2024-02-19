## 简介

___

​	这是一个轻量级的 C++ JSON工具库，提供 JSON 数据的解析、构造、序列化和访问功能。本项目借鉴了 [json11](https://github.com/dropbox/json11) ，并对其代码进行了重新的组织和重构，以提高代码的模块化、可读性和易用性。

## 特性

___

- **类型安全**：支持 JSON 标准定义的所有类型，包括 null、数字（整数和浮点数）、布尔值、字符串、数组和对象。
- **解析和序列化**：支持将 JSON 文本解析为 C++ 对象，以及将 C++ 对象序列化为 JSON 文本。
- **解析策略**：支持标准解析和带注释的解析。

## 使用

___

### 包含头文件

在 C++ 项目中包含头文件：

```cpp
#include "Json.h"
```

### 构造 JSON 值

```cpp
Json nullValue; // null
Json numberValue(42); // 数字
Json boolValue(true); // 布尔值
Json stringValue("Hello!"); // 字符串
Json arrayValue = Json::array{1, 2, 3}; // 数组
Json objectValue = Json::object{{"key", "value"}}; // 对象
```

### 解析 JSON 文本

使用 `parse` 方法将 JSON 文本解析为 `Json` 对象：

```cpp
std::string err;
Json json = Json::parse("{\"key\": \"value\"}", err);
```

### 序列化 JSON 值

使用 `dump` 方法将 `Json` 对象序列化为 JSON 文本：

```cpp
std::string jsonText = json.dump();
std::cout << jsonText << std::endl;
```

### 访问 JSON 值

可以使用重载的 `operator[]` 来访问数组元素或对象成员：

```cpp
Json::array jsonArray = {1, 2, 3};
std::cout << jsonArray[1].intValue() << std::endl; // 输出 2

Json jsonObject = Json::object{{"key", "value"}};
std::cout << jsonObject["key"].stringValue() << std::endl; // 输出 value
```

## 编译和测试

___

使用以下命令编译和运行测试程序：

```bash
make
./test
```

