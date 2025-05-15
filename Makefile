NAME = nm

LIBFT = libft/libft.a

CC = cc

CFLAGS = -Werror -Wall -Wextra

RM = rm -rf

SRCS = src/nm.c
OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	@make --no-print-directory bonus -C libft	   
	@$(CC) $(OBJS) $(LIBFT) -o $(NAME)

%.o : %.c
	@$(CC) -o $@ -c $< 

fclean : clean
	@make --no-print-directory fclean -C libft
	@$(RM) $(NAME)

clean : 
	@make --no-print-directory clean -C libft
	@$(RM) src/*.o

re : fclean all