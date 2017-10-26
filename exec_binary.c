/*
** exec_binary.c for exec_binary in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Wed May 17 00:07:02 2017 Valentin Nasraty
** Last update Sun May 21 21:52:40 2017 Valentin Nasraty
*/

#include "server.h"

short		exec_binary_with_arg_pass(char *file, int srv_sock,
				     char *bin, int fd_client)
{
  int		client_sock;
  int		saved_stdout;
  char		*cmd;

  if (access(epur_filepath(file), R_OK) == -1)
    {
      putstr_client("550 File does not exist.\r\n", fd_client);
      return (550);
    }
  if ((client_sock = wait_for_connection(srv_sock)) == ERROR_FCT)
    return (ERROR_FCT);
  putstr_client("150 Here comes the directory listing.\r\n", fd_client);
  saved_stdout = dup(1);
  if ((cmd = concat_msg(bin, file)) == NULL)
    return (ERROR_FCT);
  dup2(client_sock, 1);
  system(cmd);
  free(cmd);
  dup2(saved_stdout, 1);
  close(saved_stdout);
  shutdown(client_sock, SHUT_RDWR);
  putstr_client("226 Directory send OK.\r\n", fd_client);
  return (0);
}

short		exec_binary_with_arg_act(char *file,
				     char *bin, t_server *srv)
{
  int		saved_stdout;
  char		*cmd;

  if (access(epur_filepath(file), R_OK) == -1)
    {
      putstr_client("550 File does not exist.\r\n", srv->fd_client);
      return (550);
    }
  if (connect(srv->active_sock, (struct sockaddr *)&srv->active_client,
	      sizeof(struct sockaddr)) == -1)
    return (ERROR_FCT);
  putstr_client("150 Here comes the directory listing.\r\n", srv->fd_client);
  saved_stdout = dup(1);
  if ((cmd = concat_msg(bin, file)) == NULL)
    return (ERROR_FCT);
  dup2(srv->active_sock, 1);
  system(cmd);
  free(cmd);
  dup2(saved_stdout, 1);
  close(saved_stdout);
  putstr_client("226 Directory send OK.\r\n", srv->fd_client);
  return (0);
}
