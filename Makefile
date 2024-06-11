src=$(wildcard ./src/*.cpp)
objs=$(patsubst %.cpp, %.o, $(src))
target=OrangePiFanCtrl
CXXFLAGS += -I ./include -std=c++17 -lpthread -lwiringPi

$(target): $(objs) main.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS)
main.o: main.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean
clean:
	rm -f $(objs) main.o $(target)