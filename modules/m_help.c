/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  m_help.c: Provides help information to a user/operator.
 *
 *  Copyright (C) 2002 by the past and present ircd coders, and others.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *  USA
 *
 *  $Id: m_help.c 2044 2013-05-15 15:31:41Z michael $
 */

#include "stdinc.h"
#include "client.h"
#include "ircd.h"
#include "numeric.h"
#include "send.h"
#include "conf.h"
#include "parse.h"
#include "modules.h"
#include "irc_string.h"

#define HELPLEN 400


static void
sendhelpfile(struct Client *source_p, const char *path, const char *topic)
{
  FILE *file = NULL;
  char line[HELPLEN] = { '\0' };

  if ((file = fopen(path, "r")) == NULL)
  {
    sendto_one(source_p, form_str(ERR_HELPNOTFOUND),
               me.name, source_p->name, topic);
    return;
  }

  if (fgets(line, sizeof(line), file) == NULL)
  {
    sendto_one(source_p, form_str(ERR_HELPNOTFOUND),
               me.name, source_p->name, topic);
    return;
  }

  line[strlen(line) - 1] = '\0';
  sendto_one(source_p, form_str(RPL_HELPSTART),
             me.name, source_p->name, topic, line);

  while (fgets(line, sizeof(line), file))
  {
    line[strlen(line) - 1] = '\0';

    sendto_one(source_p, form_str(RPL_HELPTXT),
               me.name, source_p->name, topic, line);
  }

  fclose(file);
  sendto_one(source_p, form_str(RPL_ENDOFHELP),
             me.name, source_p->name, topic);
}

static void
dohelp(struct Client *source_p, char *topic)
{
  char *p = topic;
  char h_index[] = "index";
  char path[HYB_PATH_MAX + 1];
  struct stat sb;

  if (EmptyString(topic))
    topic = h_index;
  else
    for (; *p; ++p)
      *p = ToLower(*p);

  if (strpbrk(topic, "/\\"))
  {
    sendto_one(source_p, form_str(ERR_HELPNOTFOUND),
               me.name, source_p->name, topic);
    return;
  }

  if (strlen(HPATH) + strlen(topic) + 1 > HYB_PATH_MAX)
  {
    sendto_one(source_p, form_str(ERR_HELPNOTFOUND),
               me.name, source_p->name, topic);
    return;
  }

  snprintf(path, sizeof(path), "%s/%s", HPATH, topic);

  if (stat(path, &sb) < 0)
  {
    sendto_one(source_p, form_str(ERR_HELPNOTFOUND),
               me.name, source_p->name, topic);
    return;
  }

  if (!S_ISREG(sb.st_mode))
  {
    sendto_one(source_p, form_str(ERR_HELPNOTFOUND),
               me.name, source_p->name, topic);
    return;
  }

  sendhelpfile(source_p, path, topic);
}

/*
 * m_help - HELP message handler
 *      parv[0] = sender prefix
 */
static void
m_help(struct Client *client_p, struct Client *source_p,
       int parc, char *parv[])
{
  static time_t last_used = 0;

  /* HELP is always local */
  if ((last_used + ConfigFileEntry.pace_wait_simple) > CurrentTime)
  {
    /* safe enough to give this on a local connect only */
    sendto_one(source_p,form_str(RPL_LOAD2HI),
               me.name, source_p->name);
    return;
  }

  last_used = CurrentTime;

  dohelp(source_p, parv[1]);
}

/*
 * mo_help - HELP message handler
 *      parv[0] = sender prefix
 */
static void
mo_help(struct Client *client_p, struct Client *source_p,
        int parc, char *parv[])
{
  dohelp(source_p, parv[1]);
}

static struct Message help_msgtab = {
  "HELP", 0, 0, 0, MAXPARA, MFLG_SLOW, 0,
  {m_unregistered, m_help, m_ignore, m_ignore, mo_help, m_ignore}
};

static void
module_init(void)
{
  mod_add_cmd(&help_msgtab);
}

static void
module_exit(void)
{
  mod_del_cmd(&help_msgtab);
}

struct module module_entry = {
  .node    = { NULL, NULL, NULL },
  .name    = NULL,
  .version = "$Revision: 2044 $",
  .handle  = NULL,
  .modinit = module_init,
  .modexit = module_exit,
  .flags   = 0
};
