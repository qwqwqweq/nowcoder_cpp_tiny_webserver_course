# src = main.o sub.o mult.o
src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
target = app

$(target): $(obj)
	$(CXX) $(obj) -o $(target)
%.o: %.cpp
	$(CXX) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(obj)