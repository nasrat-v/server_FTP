/*
** file.c for file in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Sat May 20 12:14:18 2017 Valentin Nasraty
** Last update Sun May 21 22:10:51 2017 Valentin Nasraty
*/

#include "server.h"

char		*epur_filepath(char *file)
{
  int		i;

  i = -1;
  while (file[++i])
    {
      if (file[i] == '\n')
	file[i] = '\0';
    }
  return (file);
}

void		remove_file(char *file, int fd_client)
{
  if (access(epur_filepath(file), R_OK | W_OK) != -1)
    {
      system(concat_msg("rm -rf ", epur_filepath(file)));
      putstr_client("250 File successfully removed.\r\n", fd_client);
    }
  else
    {
      if (errno == EACCES)
	putstr_client("550 Permission denied.\r\n", fd_client);
      else
	putstr_client("550 File does not exist.\r\n", fd_client);
    }
}

void		write_file(int fd_file, int client_sock)
{
  char			buff[NB_OCTET_READ];
  ssize_t		ret_size;

  memset(buff, 0, NB_OCTET_READ);
  while ((ret_size = read(client_sock, buff,
		     (NB_OCTET_READ - 1))) == (NB_OCTET_READ - 1))
    write(fd_file, buff, (NB_OCTET_READ - 1));
  if (ret_size > 0)
    write(fd_file, buff, ret_size);
}

short		store_file_pass(char *file, int srv_sock, int fd_client)
{
  int		fd;
  int		client_sock;

  if ((fd = open(epur_filepath(file), O_RDWR | O_CREAT | O_TRUNC, 0777)) == -1)
    return (ERROR_FCT);
  if ((client_sock = wait_for_connection(srv_sock)) == ERROR_FCT)
    return (ERROR_FCT);
  putstr_client("150 Here comes the directory listing.\r\n", fd_client);
  write_file(fd, client_sock);
  putstr_client("226 Directory send OK.\r\n", fd_client);
  close(fd);
  return (0);
}

short		store_file_act(char *file, t_server *srv)
{
  int		fd;

  if ((fd = open(epur_filepath(file), O_RDWR | O_CREAT | O_TRUNC, 0777)) == -1)
    return (ERROR_FCT);
  if (connect(srv->active_sock, (struct sockaddr *)&srv->active_client,
	      sizeof(struct sockaddr)) == -1)
    return (ERROR_FCT);
  putstr_client("150 Here comes the directory listing.\r\n", srv->fd_client);
  write_file(fd, srv->active_sock);
  putstr_client("226 Directory send OK.\r\n", srv->fd_client);
  close(fd);
  return (0);
}
