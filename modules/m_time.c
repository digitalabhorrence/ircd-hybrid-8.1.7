/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  m_time.c: Sends the current time on the server.
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
 *  $Id: m_time.c 2197 2013-06-04 18:33:25Z michael $
 */

#include "stdinc.h"
#include "client.h"
#include "ircd.h"
#include "numeric.h"
#include "s_misc.h"
#include "conf.h"
#include "s_serv.h"
#include "send.h"
#include "parse.h"
#include "modules.h"
#include "packet.h"


/*
 * m_time
 *      parv[0] = sender prefix
 *      parv[1] = servername
 */
static void
m_time(struct Client *client_p, struct Client *source_p,
       int parc, char *parv[])
{
  /* this is not rate limited, so end the grace period */
  if (!IsFloodDone(source_p))
    flood_endgrace(source_p);

  /* This is safe enough to use during non hidden server mode */
  if (!ConfigServerHide.disable_remote_commands)
    if (hunt_server(client_p, source_p, ":%s TIME :%s", 1, parc, parv) != HUNTED_ISME)
      return;

  sendto_one(source_p, form_str(RPL_TIME), me.name,
             source_p->name, me.name, date(0));
}

/*
 * mo_time
 *      parv[0] = sender prefix
 *      parv[1] = servername
 */
static void
mo_time(struct Client *client_p, struct Client *source_p,
        int parc, char *parv[])
{
  if (hunt_server(client_p, source_p, ":%s TIME :%s", 1, parc, parv) == HUNTED_ISME)
    sendto_one(source_p, form_str(RPL_TIME), me.name,
               source_p->name, me.name, date(0));
}

static struct Message time_msgtab = {
  "TIME", 0, 0, 0, MAXPARA, MFLG_SLOW, 0,
  { m_unregistered, m_time, mo_time, m_ignore, mo_time, m_ignore }
};

static void
module_init(void)
{
  mod_add_cmd(&time_msgtab);
}

static void
module_exit(void)
{
  mod_del_cmd(&time_msgtab);
}

struct module module_entry = {
  .node    = { NULL, NULL, NULL },
  .name    = NULL,
  .version = "$Revision: 2197 $",
  .handle  = NULL,
  .modinit = module_init,
  .modexit = module_exit,
  .flags   = 0
};
