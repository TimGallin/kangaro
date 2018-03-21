VPATH = src/
objects = kangaro.o pouchserver.o

kangaro : $(objects)
	$(CXX) -o $@ $^

kangaro.o : kangaro.h

pouchserver.o : kangaro.h pouchserver.h

.PHONY : clean
clean :
	rm -rf $(objects)
