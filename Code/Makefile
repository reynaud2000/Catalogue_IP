SRC	=	src/*.c
	# src/menu.c	\
	# src/list.c	\
	# src/utils.c	\
	# src/command.c	\
	# src/base_sql.c	\
	# src/convertir.c	\
	# src/ncurses.c

OBJ	=	$(SRC:.c=.o)

NAME	=	catalogue_ip

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc -o $(NAME) $(SRC) $(pkg-config --cflags --libs gtk+-3.0) -lm -lsqlite3 -g3 -lncurses -g

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)
	rm -f src/*~
	rm -f include/*~
	rm -f *~

re:	fclean all