CXX = g++

CXXFLAGS = -std=c++11
# 目标可执行文件
TARGET = test
# .cpp文件
SOURCES = test.cpp Json.cpp Parse.cpp

all: $(TARGET)
$(TARGET): 
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
