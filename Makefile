CXX := g++
CXXFLAGS := -std=c++1z -Wall -Wextra -pedantic -O2

TARGET := omniparse
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

.PHONY: all run clean

all: $(TARGET).exe

$(TARGET).exe: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET).exe

clean:
	rm -f $(OBJS) $(TARGET).exe
