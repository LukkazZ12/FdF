NAME =			fdf
OBJECTS =		fdf.o
CC =			cc

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) -Llibft -lft -Lmlx_linux -lmlx_Linux -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)

%.o: %.c
	$(CC) -Wall -Wextra -Werror -I/usr/include -Imlx_linux -O3 -c $< -o $@