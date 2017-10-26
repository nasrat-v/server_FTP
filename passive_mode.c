/*
** passive_mode.c for passive_mode in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Tue May 16 10:28:20 2017 Valentin Nasraty
** Last update Sun May 21 22:53:41 2017 Valentin Nasraty
*/

#include "server.h"

int			wait_for_connection(int nsock_srv)
{
  unsigned int		sock_size;
  int			nsock_client;
  struct sockaddr_in	loc_client;

  sock_size = sizeof(loc_client);
  if ((nsock_client = accept(nsock_srv,
			     (struct sockaddr*)&loc_client, &sock_size)) == -1)
    {
      puterror("Error: accept connection failed\n");
      return (ERROR_FCT);
    }
  return (nsock_client);
}

void			print_information(char *ip_srv, int port, int fd_client)
{
  putstr_client("227 Entering Passive Mode (", fd_client);
  putstr_client(format_addr(ip_srv), fd_client);
  putstr_client(",255,", fd_client);
  putstr_client(int_to_string(port), fd_client);
  putstr_client(")\r\n", fd_client);
}

int			init_new_server(int nsock_srv)
{
  int			port;
  struct sockaddr_in	local;

  memset(&local, 0, sizeof(local));
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = INADDR_ANY;
  port = (rand() % MAX_OCTET);
  local.sin_port = htons((MAX_OCTET - 1) * MAX_OCTET + port);
  while ((bind(nsock_srv, (struct sockaddr*)(&local),
	       sizeof(struct sockaddr))) == -1)
    {
      port = (rand() % MAX_OCTET);
      local.sin_port = htons((MAX_OCTET - 1) * MAX_OCTET + port);
    }
  printf("Data-channel port <%d>\n", (MAX_OCTET - 1) * MAX_OCTET + port);
  if ((listen(nsock_srv, MAX_CLIENT)) == -1)
    {
      puterror("Error: listen failed\n");
      return (ERROR_FCT);
    }
  return (port);
}

short			pasm(int fd_client, char *ip_srv, bool *bad_arg)
{
  int			port;
  int			nsock_srv;

  *bad_arg = false;
  if ((nsock_srv = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      puterror("Error: socket creation failed\n");
      return (ERROR_FCT);
    }
  if ((port = init_new_server(nsock_srv)) == ERROR_FCT)
    return (ERROR_FCT);
  print_information(ip_srv, port, fd_client);
  return (nsock_srv);
}
