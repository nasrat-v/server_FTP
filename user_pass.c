/*
** user_pass.c for user_pass in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Tue May 16 21:07:41 2017 Valentin Nasraty
** Last update Sun May 21 19:28:04 2017 Valentin Nasraty
*/

#include "server.h"

bool		check_anonymous(t_server *srv)
{
  if (strncmp(srv->user, "Anonymous\n", 10) == 0)
    {
      putstr_client("230 Login successful.\r\n", srv->fd_client);
      return (true);
    }
  else
    putstr_client("530 Login incorrect.\r\n", srv->fd_client);
  return (false);
}

short		get_username(char *username, t_server *srv)
{
  static int	kick_it = 0;

  if (++kick_it == MAX_TRY_CONNEC)
    srv->kick = true;
  if (!username)
    {
      putstr_client("530 Permission denied.\r\n", srv->fd_client);
      if (srv->kick)
      	kill_new_process(srv->fd_client, EXIT_SUCCESS);
      return (false);
    }
  srv->user = username;
  putstr_client("331 Please specify the password.\r\n", srv->fd_client);
  return (true);
}

short		check_user(char *msg, t_server *srv)
{
  bool		bad_arg;
  char		**tab_msg;

  tab_msg = NULL;
  while (42)
    {
      if (tab_msg)
      	free(tab_msg);
      if (srv->kick)
	kill_new_process(srv->fd_client, EXIT_SUCCESS);
      if ((tab_msg = receive_msg(msg, srv->fd_client, tab_msg)) == NULL)
	return (ERROR_FCT);
      if (check_quit(tab_msg, srv->fd_client, &bad_arg) == QUIT)
	return (QUIT);
      if (strncmp(tab_msg[0], "USER", 4) == 0)
	{
	  if (get_username(tab_msg[1], srv))
	    return (check_pass(msg, srv));
	}
      else if (strncmp(tab_msg[0], "PASS", 4) == 0)
	putstr_client("503 Login with USER first.\r\n", srv->fd_client);
      else
	putstr_client("530 Please login with USER and PASS.\r\n", srv->fd_client);
    }
  return (0);
}

short		check_pass(char *msg, t_server *srv)
{
  bool		bad_arg;
  char		**tab_msg;

  tab_msg = NULL;
  while (42)
    {
      if (tab_msg)
      	free(tab_msg);
      if ((tab_msg = receive_msg(msg, srv->fd_client, tab_msg)) == NULL)
	return (ERROR_FCT);
      if (check_quit(tab_msg, srv->fd_client, &bad_arg) == QUIT)
	return (QUIT);
      if (strncmp(tab_msg[0], "PASS", 4) == 0)
	return (check_anonymous(srv)) ? (0) : (check_user(msg, srv));
      else if (strncmp(tab_msg[0], "USER", 4) == 0)
	return (get_username(tab_msg[1], srv))
	  ? (check_pass(msg, srv)) : (check_user(msg, srv));
      else
	putstr_client("530 Please login with USER and PASS.\r\n", srv->fd_client);
    }
  return (0);
}
