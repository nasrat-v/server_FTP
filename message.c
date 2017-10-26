/*
** message.c for message in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Tue May  9 18:43:47 2017 Valentin Nasraty
** Last update Sun May 21 19:46:04 2017 Valentin Nasraty
*/

#include "server.h"

bool		is_end_msg_telnet(char *msg)
{
  int		i;
  static short	n = 0;
  static short	x = 0;

  i = -1;
  while (msg[++i])
    {
      if ((n - x) > 1)
	{
	  x = 0;
	  n = 0;
	}
      if (msg[i] == '\r')
	x = 1;
      if (msg[i] == '\n' && x == 1)
	{
	  x = 0;
	  n = 0;
	  return (true);
	}
      if (x == 1)
      n += 1;
    }
  return (false);
}

bool		is_end_msg_netcat(char *msg)
{
  int		i;

  i = -1;
  while (msg[++i])
    {
      if (msg[i] == '\n')
	return (true);
    }
  return (false);
}

char		*read_msg(int fd_client, char *msg)
{
  ssize_t	ret;

  if ((ret = read(fd_client, msg, NB_OCTET_READ)) == -1)
    {
      puterror("Error: read client's socket failed\n");
      return (NULL);
    }
  else if (ret == 0)
    kill_new_process(fd_client, EXIT_SUCCESS);
  msg[ret] = '\0';
  return (msg);
}

char		*concat_msg(char *msg, char *new_msg)
{
  int		i;
  int		n;
  char		*str;

  i = -1;
  n = -1;
  if ((str = malloc(sizeof(char) * (strlen(msg) +
				    strlen(new_msg) + 1))) == NULL)
    return (NULL);
  while (msg[++i])
    str[i] = msg[i];
  while (new_msg[++n])
    str[i++] = new_msg[n];
  str[i] = '\0';
  return (str);
}

char		**receive_msg(char *msg, int fd_client, char **tab_msg)
{
  bool		end;
  char		*new_msg;

  end = false;
  if ((new_msg = malloc(sizeof(char) * NB_OCTET_READ + 1)) == NULL)
    return (NULL);
  memset(msg, 0, NB_OCTET_READ);
  while (!end)
    {
      memset(new_msg, 0, NB_OCTET_READ);
      if ((new_msg = read_msg(fd_client, new_msg)) == NULL)
      	return (NULL);
      msg = concat_msg(msg, new_msg);
      if (is_end_msg_telnet(new_msg) || is_end_msg_netcat(new_msg))
      	end = true;
    }
  putstr(msg);
  free(new_msg);
  if ((tab_msg = str_to_wordtab(msg, ' ')) == NULL)
    return (NULL);
  return (tab_msg);
}

