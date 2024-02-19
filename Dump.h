#ifndef DUMP_H
#define DUMP_H
#include<string>
#include<cmath>
#include"Value.h"
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




#endif