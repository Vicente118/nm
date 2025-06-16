NAME = nm

LIBFT = libft/libft.a

CC = cc

CFLAGS = -g # -Werror -Wall -Wextra 

RM = rm -rf

SRCS =  src/nm.c \
		src/file_handler.c \
		src/elf_parser.c \
		src/symbol_handler.c \
		src/display.c \
		src/utils.c

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	@make --no-print-directory bonus -C libft	   
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

%.o : %.c
	@$(CC) $(CFLAGS) -o $@ -c $< 

fclean : clean
	@make --no-print-directory fclean -C libft
	@$(RM) $(NAME)

clean : 
	@make --no-print-directory clean -C libft
	@$(RM) src/*.o

re : fclean all