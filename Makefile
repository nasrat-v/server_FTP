##
## Makefile for myftp in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
## 
## Made by Valentin Nasraty
## Login   <valentin.nasraty@epitech.eu>
## 
## Started on  Mon May  1 15:32:35 2017 Valentin Nasraty
## Last update Sun May 21 20:53:48 2017 Valentin Nasraty
##

SRC		= server.c \
		  message.c \
		  basic.c \
		  shell.c \
		  command.c \
		  passive_command.c \
		  passive_mode.c \
		  active_mode.c \
		  exec_binary.c \
		  user_pass.c \
		  str_to_wordtab.c \
		  directory.c \
		  file.c \
		  main.c

OBJ		= $(SRC:.c=.o)

NAME		= server

RM		= rm -f

CFLAGS		= -W -Wall -Werror -Wextra -Wundef -Wpointer-arith -Wcast-align -Wcast-qual -Wunreachable-code

CC		= gcc -o

all:		compile

compile:	$(OBJ)
		$(CC) $(NAME) $(OBJ)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

