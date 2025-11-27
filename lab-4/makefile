CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++17
DEBUG_FLAGS=-g
RELEASE_FLAGS=-O2

TARGET=a.exe
TEST_TARGET=test.exe

SOURCES=pearson/pearson.cpp empirical/empirical.cpp
HEADERS=pearson/pearson.h mix/mix.h empirical/empirical.h core/interface.h

OBJECTS=main.o $(SOURCES:.cpp=.o)
TEST_OBJECTS=test.o $(SOURCES:.cpp=.o)

all: $(TARGET)
	rm -f $(OBJECTS)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJECTS)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

test: $(TEST_TARGET)
	rm -f $(TEST_OBJECTS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

debug-test: CXXFLAGS += $(DEBUG_FLAGS)
debug-test: $(TEST_TARGET)

release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)