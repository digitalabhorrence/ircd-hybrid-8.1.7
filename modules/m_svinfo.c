/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  m_svinfo.c: Sends TS information for clock & compatibility checks.
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
 *  $Id: m_svinfo.c 2273 2013-06-18 16:00:15Z michael $
 */

#include "stdinc.h"
#include "client.h"
#include "irc_string.h"
#include "ircd.h"
#include "send.h"
#include "conf.h"
#include "log.h"
#include "parse.h"
#include "modules.h"


/*
 * ms_svinfo - SVINFO message handler
 *      parv[0] = sender prefix
 *      parv[1] = TS_CURRENT for the server
 *      parv[2] = TS_MIN for the server
 *      parv[3] = server is standalone or connected to non-TS only
 *      parv[4] = server's idea of UTC time
 */
static void
ms_svinfo(struct Client *client_p, struct Client *source_p,
          int parc, char *parv[])
{
  time_t deltat;
  time_t theirtime;

  if (MyConnect(source_p) && IsUnknown(source_p))
  {
    exit_client(source_p, source_p, "Need SERVER before SVINFO");
    return;
  }

  if (!IsServer(source_p) || !MyConnect(source_p) || parc < 5)
    return;

  if (TS_CURRENT < atoi(parv[2]) || atoi(parv[1]) < TS_MIN)
  {
    /*
     * a server with the wrong TS version connected; since we're
     * TS_ONLY we can't fall back to the non-TS protocol so
     * we drop the link  -orabidoo
     */
    sendto_realops_flags(UMODE_ALL, L_ADMIN, SEND_NOTICE,
            "Link %s dropped, wrong TS protocol version (%s,%s)",
            get_client_name(source_p, SHOW_IP), parv[1], parv[2]);
    sendto_realops_flags(UMODE_ALL, L_OPER, SEND_NOTICE,
                 "Link %s dropped, wrong TS protocol version (%s,%s)",
                 get_client_name(source_p, MASK_IP), parv[1], parv[2]);
    exit_client(source_p, source_p, "Incompatible TS version");
    return;
  }

  /*
   * since we're here, might as well set CurrentTime while we're at it
   */
  set_time(); 
  theirtime = atol(parv[4]);
  deltat = abs(theirtime - CurrentTime);

  if (deltat > ConfigFileEntry.ts_max_delta)
  {
    sendto_realops_flags(UMODE_ALL, L_ADMIN, SEND_NOTICE,
          "Link %s dropped, excessive TS delta (my TS=%lu, their TS=%lu, delta=%d)",
          get_client_name(source_p, SHOW_IP),
          (unsigned long) CurrentTime,
          (unsigned long) theirtime,
          (int) deltat);
    sendto_realops_flags(UMODE_ALL, L_OPER, SEND_NOTICE,
          "Link %s dropped, excessive TS delta (my TS=%lu, their TS=%lu, delta=%d)",
           get_client_name(source_p, MASK_IP),
           (unsigned long) CurrentTime,
           (unsigned long) theirtime,
           (int) deltat);
    ilog(LOG_TYPE_IRCD,
         "Link %s dropped, excessive TS delta (my TS=%lu, their TS=%lu, delta=%d)",
         get_client_name(source_p, SHOW_IP),
         (unsigned long) CurrentTime,
         (unsigned long) theirtime,
         (int) deltat);
    exit_client(source_p, source_p, "Excessive TS delta");
    return;
  }

  if (deltat > ConfigFileEntry.ts_warn_delta)
    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                "Link %s notable TS delta (my TS=%lu, their TS=%lu, delta=%d)",
                source_p->name,
                (unsigned long) CurrentTime,
                (unsigned long) theirtime,
                (int) deltat);
}

static struct Message svinfo_msgtab = {
  "SVINFO", 0, 0, 4, MAXPARA, MFLG_SLOW, 0,
  {m_unregistered, m_ignore, ms_svinfo, m_ignore, m_ignore, m_ignore}
};

static void
module_init(void)
{
  mod_add_cmd(&svinfo_msgtab);
}

static void
module_exit(void)
{
  mod_del_cmd(&svinfo_msgtab);
}

struct module module_entry = {
  .node    = { NULL, NULL, NULL },
  .name    = NULL,
  .version = "$Revision: 2273 $",
  .handle  = NULL,
  .modinit = module_init,
  .modexit = module_exit,
  .flags   = 0
};
