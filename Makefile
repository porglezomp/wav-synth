SRCS = main.c instruments.c
OBJS = $(SRCS:.c=.o)
EXE  = Sound.exe
FLAGS = -std=c99 -Wall -Werror -pedantic -D_USE_MATH_DEFINES -g

all: $(EXE)

$(EXE): $(OBJS)
	gcc -o $(EXE) $(OBJS) $(FLAGS)

%.o: %.c
	gcc $< -c $(FLAGS)

clean:
	rm $(OBJS)
	rm $(EXE)
