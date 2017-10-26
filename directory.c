/*
** directory.c for directory in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Wed May 17 11:36:14 2017 Valentin Nasraty
** Last update Sun May 21 19:27:39 2017 Valentin Nasraty
*/

#include "server.h"

void		set_root_directory(t_server *srv, char *rdir)
{
  DIR		*dir;

  srv->root_dir = concat_prefix_pwd_and_arg(getcwd(NULL, 0), rdir);
  if ((dir = opendir(rdir)) == NULL)
    mkdir(srv->root_dir, 0777);
  chdir(srv->root_dir);
}

void		aff_pwd(t_server *srv)
{
  putstr_client("257 \"", srv->fd_client);
  putstr_client(getcwd(NULL, 0), srv->fd_client);
  putstr_client("\"\r\n", srv->fd_client);
}

char		*epur_pathdir(char *dir)
{
  int		i;

  i = -1;
  while (dir[++i])
    {
      if (dir[i] && dir[i + 1] &&
	  dir[i] == '\r' && dir[i + 1] == '\n')
	{
	  dir[i] = '\0';
	  dir[i + 1] = '\0';
	}
      else if (dir[i] && dir[i] == '\n')
	dir[i] = '\0';
    }
  return (dir);
}

void		change_directory(char *path_dir, t_server *srv)
{
  DIR		*dir;

  putstr(epur_pathdir(path_dir));
  putstr("\n");
  if ((dir = opendir(epur_pathdir(path_dir))) != NULL)
    {
      chdir(epur_pathdir(path_dir));
      putstr_client("250 Directory successfully changed.\r\n", srv->fd_client);
      closedir(dir);
    }
  else
    putstr_client("550 Failed to change directory.\r\n", srv->fd_client);
}
