CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++17
DEBUG_FLAGS=-g
RELEASE_FLAGS=-O2

TARGET=a.exe
TEST_TARGET=test.exe

SOURCES=pirson/pirson.cpp pirson/alt_pirson.cpp mixd/mixd.cpp empirical/empirical.cpp
HEADERS=pirson/pirson.h pirson/alt_pirson.h mixd/mixd.h empirical/empirical.h

OBJECTS=main.o $(SOURCES:.cpp=.o)
TEST_OBJECTS=test.o $(SOURCES:.cpp=.o)

all: $(TARGET)
	

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJECTS)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

test: $(TEST_TARGET)
	

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

debug-test: CXXFLAGS += $(DEBUG_FLAGS)
debug-test: $(TEST_TARGET)

release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)