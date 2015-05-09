EXE = bin/a.out

CC = gcc

SRCDIR = src

SRC = $(wildcard $(SRCDIR)/*.c)

OBJDIR = obj

OBJ = $(subst $(SRCDIR), $(OBJDIR), $(SRC:.c=.o))

CFLAGS = -Wall -Werror -g

INC = -I./include

LIBS = -lSDL2 -lSDL2_ttf -lm

$(EXE): $(OBJ)
	$(CC) $^ $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $^ $(CFLAGS) $(INC) -o $@

.PHONY: clean fclean

clean:
	rm $(OBJ)

fclean: clean
	rm $(EXE)

re: fclean $(EXE)
