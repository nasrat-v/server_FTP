/*
** active_mode.c for active_mode in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Sun May 21 21:43:24 2017 Valentin Nasraty
** Last update Sun May 21 22:39:05 2017 Valentin Nasraty
*/

#include "server.h"

void			pars_active_info(t_active *act, char *info)
{
  int			i;
  int			port_sec;

  i = -1;
  while (info[++i])
    {
      if (info[i] == ',')
	info[i] = '.';
    }
  info[9] = '\0';
  info[13] = '\0';
  act->addr = info;
  port_sec = atoi(info + 10);
  act->port = atoi(info + 14);
  act->port = (port_sec * 256 + act->port);
}

short			ias(char *info, bool *bad_arg, t_server *srv)
{
  t_active		act;

  *bad_arg = false;
  if (info == NULL)
    putstr_client("500 Illegal PORT command.\r\n", srv->fd_client);
  else
    {
      pars_active_info(&act, info);
      memset(&srv->active_client, 0, sizeof(srv->active_client));
      if ((srv->active_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
	  puterror("Error: socket creation failed\n");
	  return (ERROR_FCT);
	}
      srv->active_client.sin_family = AF_INET;
      srv->active_client.sin_addr.s_addr = inet_addr(act.addr);
      srv->active_client.sin_port = htons(act.port);
      putstr_client("200 PORT command.\r\n", srv->fd_client);
    }
  return (0);
}
