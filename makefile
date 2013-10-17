CXX=g++
FLG=-g -DLINUX -D_DEBUG -fasm-blocks
SRCS=$(shell ls run/*.cpp) $(shell ls common/*.cpp) $(shell ls compiler/*.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))


scrap: $(OBJS) main.o
	@echo "Linking object files..."
	@$(CXX) $(FLG) -o scrap $(OBJS) main.o $(INC) $(LIBS)
	@echo "Done!"

test: $(OBJS) bytetest.o
	@echo "Linking object files..."
	@$(CXX) $(FLG) -o test $(OBJS) bytetest.o $(INC) $(LIBS)
	@echo "Done!"

install: scrap
	@sudo cp scrap /usr/bin/scrap
	@echo "Install successful"


%.o: %.cpp
	@echo "Compiling $<..."
	@$(CXX) $(FLG) -o $@ -c $< $(INC) $(LIBS)

clean:
	@rm -f $(OBJS) *.o test
	@echo "Clean completed"
