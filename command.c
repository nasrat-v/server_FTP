/*
** command.c for command in /home/nasrat_v/Dev/rendu/tek2/C/Systeme-Unix/Reseau/PSU_2016_myftp
** 
** Made by Valentin Nasraty
** Login   <valentin.nasraty@epitech.eu>
** 
** Started on  Wed May 10 16:03:27 2017 Valentin Nasraty
** Last update Sun May 21 22:54:38 2017 Valentin Nasraty
*/

#include "server.h"

short		check_quit(char **tab_msg, int fd_client, bool *bad_arg)
{
  if (strncmp(tab_msg[0], "QUIT", 4) == 0)
    {
      *bad_arg = false;
      putstr_client("221 Goodbye.\r\n", fd_client);
      return (QUIT);
    }
  return (0);
}

void		check_help(char **tab_msg, int fd_cl, bool *bad_arg)
{
  if (strncmp(tab_msg[0], "HELP", 4) == 0)
    {
      *bad_arg = false;
      putstr_client("214-The following commands are recognized.\r\n", fd_cl);
      putstr_client(" ABOR ACCT ALLO APPE CDUP CWD ", fd_cl);
      putstr_client(" DELE EPRT EPSV FEAT HELP LIST MDTM MKD\r\n", fd_cl);
      putstr_client(" MODE NLST NOOP OPTS PxASS PASV", fd_cl);
      putstr_client(" PORT PWD  QUIT REIN REST RETR RMD  RNFR\r\n", fd_cl);
      putstr_client(" RNTO SITE SIZE SMNT STAT STOR", fd_cl);
      putstr_client(" STOU STRU SYST TYPE USER XCUP XCWD XMKD\r\n", fd_cl);
      putstr_client(" XPWD XRMD\n214 Help OK.\r\n", fd_cl);
    }
}

void		check_pwd_noop_cdup_cwd(char **tab_msg,
					bool *bad_arg, t_server *srv)
{
  if (strncmp(tab_msg[0], "PWD", 3) == 0)
    {
      *bad_arg = false;
      aff_pwd(srv);
    }
  else if (strncmp(tab_msg[0], "NOOP", 4) == 0)
    {
      *bad_arg = false;
      putstr_client("200 NOOP ok.\r\n", srv->fd_client);
    }
  else if (strncmp(tab_msg[0], "CDUP", 4) == 0)
    {
      *bad_arg = false;
      chdir(srv->root_dir);
      putstr_client("250 Directory successfully changed.\r\n", srv->fd_client);
    }
  else if (strncmp(tab_msg[0], "CWD", 3) == 0)
    {
      *bad_arg = false;
      if (tab_msg[1])
	change_directory(tab_msg[1], srv);
      else
	putstr_client("550 Failed to change directory.\r\n", srv->fd_client);
    }
}

void		check_rm(char **tab_msg, bool *bad_arg, t_server *srv)
{
  if (strncmp(tab_msg[0], "DELE", 4) == 0)
    {
      *bad_arg = false;
      remove_file(tab_msg[1], srv->fd_client);
    }
}

short		check_cmd(char **tab_msg, t_server *srv)
{
  bool		ba;
  static int	ssk = 0;

  ba = true;
  check_pwd_noop_cdup_cwd(tab_msg, &ba, srv);
  check_help(tab_msg, srv->fd_client, &ba);
  if (check_quit(tab_msg, srv->fd_client, &ba) == QUIT) return (QUIT);
  if (strncmp(tab_msg[0], "PASV", 4) == 0)
    {
      srv->actv_mode = (srv->pasv_mode = true) ? false : false;
      if ((ssk = pasm(srv->fd_client, srv->ip_srv, &ba)) == 1) return (1);
    }
  else if (strncmp(tab_msg[0], "PORT", 4) == 0)
    {
      srv->actv_mode = (srv->pasv_mode = false) ? true : true;
      if (ias(tab_msg[1], &ba, srv) == ERROR_FCT) return (ERROR_FCT);
    }
  if (tab_msg[1] == NULL) check_list(tab_msg, ssk, &ba, srv);
  else
    {
      check_rm(tab_msg, &ba, srv);
      check_stor(tab_msg, ssk, &ba, srv);
    }
  if (ba) putstr_client("500 Unknown command.\r\n", srv->fd_client);
  return (0);
}
