EXE = a.out

CC = gcc

SRCDIR = src

SRC = $(wildcard $(SRCDIR)/*.c)

OBJDIR = obj

OBJ = $(subst $(SRCDIR), $(OBJDIR), $(SRC:.c=.o))

CFLAGS = -Wall -Werror -g

INC = -I./include

LIBS = -lSDL2

$(EXE): $(OBJ)
	$(CC) $^ $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $^ $(CFLAGS) $(INC) -o $@

.PHONY: clean fclean

clean:
	rm $(OB)

fclean: clean
	rm $(EXE)
