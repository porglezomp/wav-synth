SRCS = main.c
OBJS = $(SRCS:.c=.o)
EXE  = Sound.exe

all: $(EXE)

$(EXE): $(OBJS)
	gcc -o $(EXE) $(OBJS)

%.o: %.c
	gcc $< -c
