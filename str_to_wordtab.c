/*
** str_to_wordtab.c for str_to_wordtab in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Tue May 16 21:20:21 2017 Valentin Nasraty
** Last update Fri May 19 12:25:01 2017 Valentin Nasraty
*/

#include "server.h"

char	**epur_arg(char **tab)
{
  int	i;
  int	n;

  i = -1;
  while (tab[++i])
    {
      n = -1;
      while (tab[i][++n])
	{
	  if (tab[i][n] && tab[i][n + 1] &&
	      tab[i][n] == '\r' && tab[i][n + 1] == '\n')
	    {
	      tab[i][n] = '\n';
	      tab[i][n + 1] = '\0';
	    }
	}
    }
  return (tab);
}

int	nb_mot(char *str, char sep)
{
  int	i;
  int	nb;

  i = 0;
  nb = 1;
  while (str[i] != 0)
    {
      if (str[i] == sep)
	nb = nb + 1;
      i = i + 1;
    }
  return (nb);
}

char	**str_to_wordtab(char *str, char sep)
{
  char	**tab;
  int	i;
  int	j;

  if ((tab = malloc((nb_mot(str, sep) + 1) * sizeof(char*))) == NULL)
    return (NULL);
  tab[nb_mot(str, sep)] = NULL;
  i = 0;
  j = 1;
  tab[0] = str;
  while (str[i])
    {
      if (str[i] == sep)
	{
	  str[i] = 0;
	  i += 1;
	  tab[j] = &str[i];
	  j += 1;
	}
      i += 1;
    }
  return (epur_arg(tab));
}
