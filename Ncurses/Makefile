SRC	=	src/main.c	\
		src/menu.c	\
		src/list.c	\
		src/utils.c	\
		src/command.c	\
		src/handle_database.c	\
		src/convert.c	\
		src/ncurses.c

# PTESTS	=	src/my_putstr.c	\
		src/my_strcmp.c \
		src/my_strlen.c	\
		tests/test_putstr.c	\
		tests/test_strcmp.c	\
		tests/test_strlen.c

OBJ	=	$(SRC:.c=.o)

NAME	=	catalogue_ip

# TNAME	=	unit_tests

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc -o $(NAME) $(SRC) -lm -lsqlite3 -g3 -lncurses $(pkg-config --cflags gtk+-3.0) -g

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)
	rm -f src/*~
	rm -f include/*~
	rm -f *~
# rm -f $(TNAME)
# rm -f tests/*~
# rm -f *.gcda
# rm -f *.gcno

re:	fclean all

# tests_run:	$(PTESTS)
# 		gcc -o $(TNAME) $(PTESTS) --coverage -lcriterion
# 		./unit_tests