/*
** passive_command.c for passive_command in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Tue May 16 23:35:31 2017 Valentin Nasraty
** Last update Sun May 21 22:31:43 2017 Valentin Nasraty
*/

#include "server.h"

char		*concat_prefix_pwd_and_arg(char *pwd, char *file)
{
  int		i;
  int		n;
  char		*str;

  i = -1;
  n = -1;
  if ((str = malloc(sizeof(char) * (strlen(pwd) + strlen(file) + 1))) == NULL)
    return (NULL);
  while (pwd[++i])
    str[i] = pwd[i];
  str[i++] = '/';
  while (file[++n])
    str[i++] = file[n];
  str[i] = '\0';
  return (str);
}

short		exec_bin_pass(t_server *srv, int srv_sock,
			      char **tab_msg, char *bin)
{
  int		ret_value;

  if (tab_msg[1])
    {
      if (srv->pasv_mode == true)
	{
	  if ((ret_value = exec_binary_with_arg_pass(tab_msg[1], srv_sock,
						     bin, srv->fd_client)) == 0)
	    srv->pasv_mode = false;
	}
      else
	if ((ret_value = exec_binary_with_arg_act(tab_msg[1], bin, srv)) == 0)
	  srv->actv_mode = false;
      return (ret_value);
    }
  if (srv->pasv_mode == true)
    {
      if ((ret_value = exec_binary_with_arg_pass(getcwd(NULL, 0), srv_sock,
						 bin, srv->fd_client)) == 0)
	srv->pasv_mode = false;
    }
  else
    if ((ret_value = exec_binary_with_arg_act(getcwd(NULL, 0), bin, srv)) == 0)
      srv->actv_mode = false;
  return (ret_value);
}

short		check_list(char **tab_msg, int srv_sock,
			   bool *bad_arg, t_server *srv)
{
  if (strncmp(tab_msg[0], "LIST", 4) == 0)
    {
      *bad_arg = false;
      if (srv->pasv_mode || srv->actv_mode)
	{
	  if (exec_bin_pass(srv, srv_sock, tab_msg, "/bin/ls -l ") == ERROR_FCT)
	    return (ERROR_FCT);
	}
      else
	putstr_client("425 Use PORT or PASV first.\r\n", srv->fd_client);
    }
  return (0);
}

short		check_retr(char **tab_msg, int srv_sock,
			   bool *bad_arg, t_server *srv)
{
  if (strncmp(tab_msg[0], "RETR", 4) == 0)
    {
      *bad_arg = false;
      if (srv->pasv_mode || srv->actv_mode)
	{
	  if (exec_bin_pass(srv, srv_sock, tab_msg, "/bin/cat ") == ERROR_FCT)
	    return (ERROR_FCT);
	}
      else
	putstr_client("425 Use PORT or PASV first.\r\n", srv->fd_client);
    }
  else
    check_list(tab_msg, srv_sock, bad_arg, srv);
  return (0);
}

short		check_stor(char **tab_msg, int srv_sock,
			   bool *bad_arg, t_server *srv)
{
  if (strncmp(tab_msg[0], "STOR", 4) == 0)
    {
      *bad_arg = false;
      if (srv->pasv_mode || srv->actv_mode)
	{
	  if (srv->pasv_mode)
	    {
	      if (store_file_pass(tab_msg[1], srv_sock,
				  srv->fd_client) == ERROR_FCT)
		return (ERROR_FCT);
	      srv->pasv_mode = false;
	    }
	  else
	    {
	      if (store_file_act(tab_msg[1], srv) == ERROR_FCT)
		return (ERROR_FCT);
	      srv->actv_mode = false;
	    }
	}
      else
	putstr_client("425 Use PORT or PASV first.\r\n", srv->fd_client);
    }
  else
    check_retr(tab_msg, srv_sock, bad_arg, srv);
  return (0);
}
