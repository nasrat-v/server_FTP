/*
** main.c for myftp in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Mon May  1 15:48:01 2017 Valentin Nasraty
** Last update Sun May 21 19:22:40 2017 Valentin Nasraty
*/

#include "server.h"
#include <stdio.h>

void		control_c(__attribute__((unused))int signo)
{
  if (getpid() != srv.pid_father)
    kill_new_process(srv.fd_client, EXIT_SUCCESS);
  close(srv.srv_socket);
  free(srv.all_fd_client);
  exit(EXIT_SUCCESS);
}

short		is_digit(char *port)
{
  int		i;

  i = -1;
  while (port[++i])
    if (port[i] < 48 || port[i] > 57)
      return (ERROR_PARAM);
  return (0);
}

short		check_params(int ac, char *port)
{
  if (ac != 3 || (is_digit(port) == ERROR_PARAM))
    {
      puterror("Usage : ./server port path\n");
      return (ERROR_PARAM);
    }
  return (0);
}

int		main(int ac, char **av)
{
  if (check_params(ac, av[1]) == ERROR_PARAM)
    return (ERROR_PARAM);
  srand(time(NULL));
  signal(SIGCHLD, SIG_IGN);
  if (init_server(&srv, atoi(av[1]), av[2]) == ERROR_FCT)
    return (ERROR_FCT);
  signal(SIGINT, control_c);
  if (accept_connection(&srv) == ERROR_FCT)
    return (ERROR_FCT);
  return (0);
}
