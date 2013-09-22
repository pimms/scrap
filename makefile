CXX=g++
FLG=-g -std=gnu++0x -DLINUX -D_DEBUG
SRCS=$(shell ls run/*.cpp) $(shell ls *.cpp) $(shell ls common/*.cpp) $(shell ls compiler/*.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

all: $(OBJS)
	@echo "Linking object files..."
	@$(CXX) $(FLG) -o scrap $(OBJS) $(INC) $(LIBS)
	@echo "Done!"

%.o: %.cpp
	@echo "Compiling $<..."
	@$(CXX) $(FLG) -o $@ -c $< $(INC) $(LIBS)

clean:
	@rm -f $(OBJS)
	@echo "Clean completed"
