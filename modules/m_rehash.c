/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  m_rehash.c: Re-reads the configuration file.
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
 *  $Id: m_rehash.c 2344 2013-07-03 18:17:10Z michael $
 */

#include "stdinc.h"
#include "client.h"
#include "irc_string.h"
#include "ircd.h"
#include "list.h"
#include "numeric.h"
#include "irc_res.h"
#include "conf.h"
#include "log.h"
#include "send.h"
#include "parse.h"
#include "modules.h"
#include "motd.h"


/*
 * mo_rehash - REHASH message handler
 *
 */
static void
mo_rehash(struct Client *client_p, struct Client *source_p,
          int parc, char *parv[])
{
  int found = 0;

  if (!HasOFlag(source_p, OPER_FLAG_REHASH))
  {
    sendto_one(source_p, form_str(ERR_NOPRIVS),
               me.name, source_p->name, "rehash");
    return;
  }

  if (!EmptyString(parv[1]))
  {
    if (irccmp(parv[1], "DNS") == 0)
    {
      sendto_one(source_p, form_str(RPL_REHASHING), me.name, source_p->name, "DNS");
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "%s is rehashing DNS",
                           get_oper_name(source_p));
      restart_resolver();   /* re-read /etc/resolv.conf AGAIN?
                               and close/re-open res socket */
      found = 1;
    }
    else if (irccmp(parv[1], "MOTD") == 0)
    {
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "%s is forcing re-reading of MOTD files",
                           get_oper_name(source_p));
      motd_recache();
      found = 1;
    }

    if (found)
    {
      ilog(LOG_TYPE_IRCD, "REHASH %s From %s",
           parv[1], get_oper_name(source_p));
      return;
    }
    else
    {
      sendto_one(source_p, ":%s NOTICE %s :%s is not a valid option. "
                 "Choose from DNS, MOTD",
                 me.name, source_p->name, parv[1]);
      return;
    }
  }
  else
  {
    sendto_one(source_p, form_str(RPL_REHASHING),
               me.name, source_p->name, ConfigFileEntry.configfile);
    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "%s is rehashing server config file",
                         get_oper_name(source_p));
    ilog(LOG_TYPE_IRCD, "REHASH From %s",
         get_oper_name(source_p));
    rehash(0);
  }
}

static struct Message rehash_msgtab = {
  "REHASH", 0, 0, 0, MAXPARA, MFLG_SLOW, 0,
  {m_unregistered, m_not_oper, m_ignore, m_ignore, mo_rehash, m_ignore}
};

static void
module_init(void)
{
  mod_add_cmd(&rehash_msgtab);
}

static void
module_exit(void)
{
  mod_del_cmd(&rehash_msgtab);
}

struct module module_entry = {
  .node    = { NULL, NULL, NULL },
  .name    = NULL,
  .version = "$Revision: 2344 $",
  .handle  = NULL,
  .modinit = module_init,
  .modexit = module_exit,
  .flags   = 0
};
