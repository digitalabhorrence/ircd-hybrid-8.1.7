/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  m_topic.c: Sets a channel topic.
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
 *  $Id: m_topic.c 2020 2013-05-12 16:56:15Z michael $
 */

#include "stdinc.h"
#include "list.h"
#include "channel.h"
#include "channel_mode.h"
#include "client.h"
#include "hash.h"
#include "irc_string.h"
#include "ircd.h"
#include "numeric.h"
#include "send.h"
#include "s_serv.h"
#include "parse.h"
#include "modules.h"
#include "packet.h"


/* m_topic()
 *  parv[0] = sender prefix
 *  parv[1] = channel name
 *  parv[2] = new topic, if setting topic
 */
static void
m_topic(struct Client *client_p, struct Client *source_p,
        int parc, char *parv[])
{
  struct Channel *chptr = NULL;

  if (EmptyString(parv[1]))
  {
    sendto_one(source_p, form_str(ERR_NEEDMOREPARAMS),
               me.name, source_p->name, "TOPIC");
    return;
  }

  if (!IsFloodDone(source_p))
    flood_endgrace(source_p);

  if ((chptr = hash_find_channel(parv[1])) == NULL)
  {
    sendto_one(source_p, form_str(ERR_NOSUCHCHANNEL),
               me.name, source_p->name, parv[1]);
    return;
  }

  /* setting topic */
  if (parc > 2)
  {
    struct Membership *ms;

    if ((ms = find_channel_link(source_p, chptr)) == NULL)
    {
      sendto_one(source_p, form_str(ERR_NOTONCHANNEL), me.name,
                 source_p->name, parv[1]);
      return;
    }

    if (!(chptr->mode.mode & MODE_TOPICLIMIT) ||
        has_member_flags(ms, CHFL_CHANOP|CHFL_HALFOP))
    {
      char topic_info[USERHOST_REPLYLEN]; 

      snprintf(topic_info, sizeof(topic_info), "%s!%s@%s", source_p->name,
               source_p->username, source_p->host);
      set_channel_topic(chptr, parv[2], topic_info, CurrentTime, 1);

      sendto_server(client_p, CAP_TS6, NOCAPS,
                    ":%s TOPIC %s :%s",
                    ID(source_p), chptr->chname,
                    chptr->topic);
      sendto_server(client_p, NOCAPS, CAP_TS6,
                    ":%s TOPIC %s :%s",
                    source_p->name, chptr->chname,
                    chptr->topic);
      sendto_channel_local(ALL_MEMBERS, 0,
                           chptr, ":%s!%s@%s TOPIC %s :%s",
                           source_p->name,
                           source_p->username,
                           source_p->host,
                           chptr->chname, chptr->topic);
    }
    else
      sendto_one(source_p, form_str(ERR_CHANOPRIVSNEEDED),
                 me.name, source_p->name, chptr->chname);
  }
  else /* only asking for topic */
  {
    if (!SecretChannel(chptr) || IsMember(source_p, chptr))
    {
      if (chptr->topic[0] == '\0')
        sendto_one(source_p, form_str(RPL_NOTOPIC),
                   me.name, source_p->name, chptr->chname);
      else
      {
        sendto_one(source_p, form_str(RPL_TOPIC),
                   me.name, source_p->name,
                   chptr->chname, chptr->topic);
        sendto_one(source_p, form_str(RPL_TOPICWHOTIME),
                   me.name, source_p->name, chptr->chname,
                   chptr->topic_info,
                   chptr->topic_time);
      }
    }
    else
      sendto_one(source_p, form_str(ERR_NOTONCHANNEL),
                 me.name, source_p->name, chptr->chname);
  }
}

static void
ms_topic(struct Client *client_p, struct Client *source_p,
         int parc, char *parv[])
{
  struct Channel *chptr = NULL;
  const char *from, *to;
  char topic_info[USERHOST_REPLYLEN];

  if (IsCapable(source_p->from, CAP_TS6) && HasID(source_p))
  {
    from = me.id;
    to = source_p->id;
  }
  else
  {
    from = me.name;
    to = source_p->name;
  }

  if (EmptyString(parv[1]))
  {
    sendto_one(source_p, form_str(ERR_NEEDMOREPARAMS),
               from, to, "TOPIC");
    return;
  }

  if ((chptr = hash_find_channel(parv[1])) == NULL)
  {
    sendto_one(source_p, form_str(ERR_NOSUCHCHANNEL),
               from, to, parv[1]);
    return;
  }

  if (!IsClient(source_p))
    strlcpy(topic_info, source_p->name, sizeof(topic_info));
  else
    snprintf(topic_info, sizeof(topic_info), "%s!%s@%s", source_p->name,
             source_p->username, source_p->host);
  set_channel_topic(chptr, parv[2], topic_info, CurrentTime, 0);

  sendto_server(client_p, CAP_TS6, NOCAPS, ":%s TOPIC %s :%s",
                ID(source_p), chptr->chname,
                chptr->topic);
  sendto_server(client_p, NOCAPS, CAP_TS6, ":%s TOPIC %s :%s",
                source_p->name, chptr->chname,
                chptr->topic);

  if (!IsClient(source_p))
    sendto_channel_local(ALL_MEMBERS, 0, chptr, ":%s TOPIC %s :%s",
                         source_p->name,
                         chptr->chname, chptr->topic);

  else
    sendto_channel_local(ALL_MEMBERS, 0, chptr, ":%s!%s@%s TOPIC %s :%s",
                         source_p->name,
                         source_p->username,
                         source_p->host,
                         chptr->chname, chptr->topic);
}


static struct Message topic_msgtab = {
  "TOPIC", 0, 0, 2, MAXPARA, MFLG_SLOW, 0,
  {m_unregistered, m_topic, ms_topic, m_ignore, m_topic, m_ignore}
};

static void
module_init(void)
{
  mod_add_cmd(&topic_msgtab);
}

static void
module_exit(void)
{
  mod_del_cmd(&topic_msgtab);
}

struct module module_entry = {
  .node    = { NULL, NULL, NULL },
  .name    = NULL,
  .version = "$Revision: 2020 $",
  .handle  = NULL,
  .modinit = module_init,
  .modexit = module_exit,
  .flags   = 0
};
