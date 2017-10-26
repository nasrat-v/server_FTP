/*
** basic.c for basic in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Tue May  9 23:50:25 2017 Valentin Nasraty
** Last update Sun May 21 19:22:07 2017 Valentin Nasraty
*/

#include "server.h"

void		putstr_client(char *str, int fd_client)
{
  int		i;

  i = -1;
  while (str[++i])
    write(fd_client, &str[i], 1);
}

void		putstr(char *str)
{
  int		i;

  i = -1;
  while (str[++i])
    write(1, &str[i], 1);
}

void		puterror(char *str)
{
  int		i;

  i = -1;
  while (str[++i])
    write(2, &str[i], 1);
}

char		*int_to_string(int nb)
{
  int		i;
  int		x;
  char		*str;

  i = 0;
  x = nb;
  while (x > 0)
    {
      x /= 10;
      i += 1;
    }
  if ((str = malloc(sizeof(char) * (i + 1))) == NULL)
    return (NULL);
  snprintf(str, (i + 1), "%d", nb);
  return (str);
}

char			*format_addr(char *ip)
{
  int			i;

  i = -1;
  while (ip[++i])
    {
      if (ip[i] == '.')
	ip[i] = ',';
    }
  return (ip);
}
