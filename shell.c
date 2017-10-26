/*
** shell.c for shell in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Thu May 11 22:24:03 2017 Valentin Nasraty
** Last update Fri May 19 10:16:01 2017 Valentin Nasraty
*/

#include "server.h"

short		ftp__shell(t_server *srv)
{
  short		ret_val;
  char		**tab_msg;
  char		*msg;

  tab_msg = NULL;
  if ((msg = malloc(sizeof(char) * NB_OCTET_READ + 1)) == NULL)
    return (ERROR_FCT);
  if (check_user(msg, srv) == QUIT)
    return (QUIT);
  while ("la mere de quesseveur est une grosse folle")
    {
      if (tab_msg)
      	free(tab_msg);
      if ((tab_msg = receive_msg(msg, srv->fd_client, tab_msg)) == NULL)
	return (ERROR_FCT);
      if ((ret_val = check_cmd(tab_msg, srv)) == QUIT)
	{
	  free(msg);
	  free(tab_msg);
	  return (QUIT);
	}
      else if (ret_val == ERROR_FCT)
	return (ERROR_FCT);
    }
   return (0);
}
