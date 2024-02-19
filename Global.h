#ifndef GLOBAL_H
#define GLOBAL_H
#include"Value.h"

static const int max_depth = 200;

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


#endif