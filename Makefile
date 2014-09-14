SRCS = main.c instruments.c note.c loadscore.c
OBJS = $(SRCS:.c=.o)
EXE  = Sound.exe
FLAGS = -std=c99 -Wall -Werror -pedantic -D_USE_MATH_DEFINES -g
CSCORE = mario.cscore
PYSCORE = $(CSCORE:.cscore=.pyscore)

all: $(EXE) $(CSCORE)

$(EXE): $(OBJS)
	gcc -o $(EXE) $(OBJS) $(FLAGS)

%.o: %.c
	gcc $< -c $(FLAGS)

clean:
	rm $(OBJS)
	rm $(EXE)

%.cscore: %.pyscore
	python3 converter.py $<
