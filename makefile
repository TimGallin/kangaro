VPATH = src/
objects = kangaro.o pouchserver.o httprecver.o httpparams.o

kangaro : $(objects)
	$(CXX) -lpthread -o $@ $^

kangaro.o : kangaro.h

pouchserver.o : kangaro.h pouchserver.h

httprecver.o : kangaro.h httprecver.h

httpparams.o : kangaro.h httpparams.h

.PHONY : clean
clean :
	rm -rf $(objects)
