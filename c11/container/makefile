DIR = ./
CXX = g++
AR = ar
CXXFLAGS = -g -O2 -Wall -pipe -std=c++11
INC = -I.
CPPS = $(shell find -L $(DIR) -name '*.cpp')
OBJS = $(patsubst %.cpp, %.o, $(CPPS))
LIB = -lrt -lpthread -lboost_coroutine

TARGET = main

all : $(TARGET)
$(TARGET) : $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -o $@ $(INC) $(LIB)
%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(INC)

proto :
	@cd GDP/PBData/ && sh build_proto.sh
	@cd ServiceProto/ && sh build_proto.sh

clean :
	@rm -f *.gch $(TARGET)
	@find $(DIR) -name '*.o' | xargs rm -f
