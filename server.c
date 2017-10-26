/*
** server.c for server in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/my_ftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Mon Apr 24 10:43:46 2017 Valentin Nasraty
** Last update Sun May 21 21:57:47 2017 Valentin Nasraty
*/

#include "server.h"

void		kill_new_process(int fd_client, int ret_value)
{
  shutdown(fd_client, SHUT_RDWR);
  exit(ret_value);
}

short		create_new_process(t_server *srv)
{
  pid_t		pid;
  short		ret_value;

  if ((pid = fork()) == -1)
    {
      puterror("Error: unable to create new process\n");
      return (ERROR_FCT);
    }
  else if (pid == 0)
    {
      putstr("Connection accepted\n");
      if ((ret_value = ftp__shell(srv)) == QUIT)
	kill_new_process(srv->fd_client, EXIT_SUCCESS);
      else if (ret_value == ERROR_FCT)
	kill_new_process(srv->fd_client, EXIT_FAILURE);
    }
  else
    accept_connection(srv);
  return (0);
}

int		*add_client(int fd_client, int *all_fd_client)
{
  static int	len = -1;

  len += 1;
  if ((all_fd_client = realloc(all_fd_client,
			       sizeof(int) * (len + 1))) == NULL)
    {
      puterror("Error: unable to add new client\n");
      return (NULL);
    }
  all_fd_client[len] = fd_client;
  return (all_fd_client);
}

short		accept_connection(t_server *srv)
{
  unsigned int	sock_size;

  sock_size = sizeof(srv->client);
  if ((srv->fd_client = accept(srv->srv_socket,
			       (struct sockaddr*)&srv->client, &sock_size)) == -1)
    {
      puterror("Error: accept connection failed\n");
      return (ERROR_FCT);
    }
  getsockname(srv->fd_client, (struct sockaddr*)(&srv->client), &sock_size);
  srv->ip_srv = inet_ntoa(srv->client.sin_addr);
  srv->port_client = ntohs(srv->client.sin_port);
  if ((srv->all_fd_client = add_client(srv->fd_client,
				       srv->all_fd_client)) == NULL)
    return (ERROR_FCT);
  putstr_client("220 (vsFTPd 3.0.0)\r\n", srv->fd_client);
  return (create_new_process(srv));
}

short		init_server(t_server *srv, int port, char *rdir)
{
  if ((srv->srv_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      puterror("Error: socket creation failed\n");
      return (ERROR_FCT);
    }
  memset(&srv->sin, 0, sizeof(srv->sin));
  srv->sin.sin_family = AF_INET;
  srv->sin.sin_port = htons(port);
  srv->sin.sin_addr.s_addr = INADDR_ANY;
  if ((bind(srv->srv_socket,
	    (struct sockaddr*)(&srv->sin), sizeof(struct sockaddr))) == -1)
    {
      puterror("Error: bind failed\n");
      return (ERROR_FCT);
    }
  if ((listen(srv->srv_socket, MAX_CLIENT)) == -1)
    {
      puterror("Error: listen failed\n");
      return (ERROR_FCT);
    }
  srv->pid_father = getpid();
  srv->pasv_mode = false;
  srv->actv_mode = false;
  set_root_directory(srv, rdir);
  return (0);
}
