/*
** server.h for serverh in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/my_ftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Mon Apr 24 10:43:40 2017 Valentin Nasraty
** Last update Sun May 21 22:53:03 2017 Valentin Nasraty
*/

#ifndef __SERVER_H__
# define __SERVER_H__
# define _GNU_SOURCE

# define MAX_TRY_CONNEC	3
# define MAX_CLIENT	1000

# define MAX_OCTET	256
# define NB_OCTET_READ	5

# define ERROR_FCT	1
# define ERROR_PARAM	2
# define QUIT		221

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct		s_active
{
  char			*addr;
  int			port;
}			t_active;

typedef struct		s_server
{
  struct sockaddr_in	sin;
  struct sockaddr_in	client;
  struct sockaddr_in	active_client;
  int			active_sock;
  int			fd_client;
  int			srv_socket;
  int			*all_fd_client;
  pid_t			pid_father;
  char			*ip_srv;
  int			port_client;
  bool			pasv_mode;
  char			*user;
  bool			kick;
  char			*root_dir;
  bool			actv_mode;
}			t_server;

/* GLOBAL */
t_server		srv;

/* FILE */
char			*epur_filepath(char *file);
void			remove_file(char *file, int fd_client);
void			write_file(int fd_client, int client_sock);
short			store_file_pass(char *file, int srv_sock, int fd_client);
short			store_file_act(char *file, t_server *srv);

/* DIRECTORY */
void			set_root_directory(t_server *srv, char *rdir);
void			aff_pwd(t_server *srv);
char			*epur_pathdir(char *dir);
void			change_directory(char *dir, t_server *srv);

/* STR TO WORDTAB */
char			**epur_arg(char **tab);
int			nb_mot(char *str, char sep);
char			**str_to_wordtab(char *str, char sep);
void			show_wordtab(char **tab);

/* USER PASS */
bool			check_anonymous(t_server *srv);
short			get_username(char *username, t_server *srv);
short			check_user(char *msg, t_server *srv);
short			check_pass(char *msg, t_server *srv);

/* EXEC BINARY */
short			exec_binary_with_arg_pass(char *file, int srv_sock,
					     char *bin, int fd_client);
short			exec_binary_with_arg_act(char *file, char *bin,
						 t_server *srv);

/* MESSAGE */
bool			is_end_msg_telnet(char *msg);
bool			is_end_msg_netcat(char *msg);
char			*read_msg(int fd_client, char *msg);
char			*concat_msg(char *msg, char *new_msg);
char			**receive_msg(char *msg, int fd_client, char **tab_msg);

/* BASIC FUNCTIONS */
void			putstr_client(char *str, int fd_client);
void			putstr(char *str);
void			puterror(char *str);
char			*int_to_string(int nb);
char			*format_addr(char *ip);

/* COMMAND */
short			check_quit(char **tab_msg, int fd_client, bool *bad_arg);
void			check_help(char **tab_msg, int fd_cl, bool *bad_arg);
void			check_pwd_noop_cdup_cwd(char **tab_msg,
						bool *bad_arg, t_server *srv);
void			check_rm(char **tab_msg, bool *bad_arg, t_server *srv);
short			check_cmd(char **tab_msg, t_server *srv);

/* PASSIVE COMMAND */
char			*concat_prefix_pwd_and_arg(char *pwd, char *file);
short			exec_bin(t_server *srv, int srv_sock,
				 char **tab_msg, char *bin);
short			check_list(char **tab_msg, int srv_sock,
				   bool *bad_arg, t_server *srv);
short			check_retr(char **tab_msg, int srv_sock,
				   bool *bad_arg, t_server *srv);
short			check_stor(char **tab_msg, int srv_sock,
				   bool *bad_arg, t_server *srv);

/* PASSIVE MODE */
int			wait_for_connection(int nsock_srv);
void			print_information(char *ip_srv, int port, int fd_client);
int			init_new_server(int nsock_srv);
short			pasm(int fd_client, char *ip_srv, bool *bad_arg);
short			actv_pasv_mode(int srv_sock, char **tab_msg,
				       bool *bad_arg, t_server *srv);

/* ACTIVE MODE */
void			pars_active_info(t_active *act, char *info);
short			ias(char *info, bool *bad_arg,
					   t_server *srv);

/* SHELL */
short			ftp__shell(t_server *srv);

/* SERVER */
void			kill_new_process(int fd_client, int ret_value);
short			create_new_process(t_server *srv);
int			*add_client(int fd_client, int *all_fd_client);
short			accept_connection(t_server *srv);
short			init_server(t_server *srv, int port, char *root_dir);

/* MAIN */
void			control_c(int signo);
short			is_digit(char *port);
short			check_params(int ac, char *port);

#endif /* !__SERVER_H__ */
