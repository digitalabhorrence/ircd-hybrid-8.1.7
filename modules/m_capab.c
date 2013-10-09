/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  m_capab.c: Negotiates capabilities with a remote server.
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
 *  $Id: m_capab.c 1983 2013-05-08 20:44:53Z michael $
 */

#include "stdinc.h"
#include "client.h"
#include "irc_string.h"
#include "s_serv.h"
#include "conf.h"
#include "parse.h"
#include "modules.h"


/*
 * mr_capab - CAPAB message handler
 *      parv[0] = sender prefix
 *      parv[1] = space-separated list of capabilities
 *
 */
static void
mr_capab(struct Client *client_p, struct Client *source_p,
         int parc, char *parv[])
{
  int i = 0;
  unsigned int cap = 0;
  char *p = NULL;
  char *s = NULL;

  if (client_p->localClient->caps && !(IsCapable(client_p, CAP_TS6)))
  {
    exit_client(client_p, client_p, "CAPAB received twice");
    return;
  }

  SetCapable(client_p, CAP_CAP);

  for (i = 1; i < parc; ++i)
    for (s = strtoken(&p, parv[i], " "); s;
         s = strtoken(&p,    NULL, " "))
        if ((cap = find_capability(s)))
          SetCapable(client_p, cap);
}

static struct Message capab_msgtab = {
  "CAPAB", 0, 0, 2, MAXPARA, MFLG_SLOW, 0,
  { mr_capab, m_ignore, m_ignore, m_ignore, m_ignore, m_ignore }
};

static void
module_init(void)
{
  mod_add_cmd(&capab_msgtab);
}

static void
module_exit(void)
{
  mod_del_cmd(&capab_msgtab);
}

struct module module_entry = {
  .node    = { NULL, NULL, NULL },
  .name    = NULL,
  .version = "$Revision: 1983 $",
  .handle  = NULL,
  .modinit = module_init,
  .modexit = module_exit,
  .flags   = 0
};
