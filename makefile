CC=gcc
INCS=
FLG=-g
OUTPUT=scrap

SRC=$(shell find src/ | egrep ".*\.c$$")
OBJ=$(SRC:.c=.o)

all:scrap

scrap:$(OBJ)
	@echo "LINK $(OUTPUT)"
	@$(CC) $(FLG) -o $(OUTPUT) $(OBJ) $(INCS) $(LIBS)

%.o: %.c
	@echo "CC   $<"
	@$(CC) $(FLG) -o $@ -c $< $(INCS) $(LIBS)

clean:
	@rm -f $(OBJ) $(OUTPUT)
