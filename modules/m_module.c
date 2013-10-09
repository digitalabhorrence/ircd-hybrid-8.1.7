/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
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
 */

/*! \file m_module.c
 * \brief Includes required functions for processing the MODULE command.
 * \version $Id: m_module.c 1834 2013-04-19 19:50:27Z michael $
 */

#include "stdinc.h"
#include "list.h"
#include "client.h"
#include "irc_string.h"
#include "ircd.h"
#include "numeric.h"
#include "conf.h"
#include "log.h"
#include "s_user.h"
#include "send.h"
#include "parse.h"
#include "modules.h"
#include "packet.h"



/*! \brief MODULE command handler (called by operators)
 *
 * \param client_p Pointer to allocated Client struct with physical connection
 *                 to this server, i.e. with an open socket connected.
 * \param source_p Pointer to allocated Client struct from which the message
 *                 originally comes from.  This can be a local or remote client.
 * \param parc     Integer holding the number of supplied arguments.
 * \param parv     Argument vector where parv[0] .. parv[parc-1] are non-NULL
 *                 pointers.
 * \note Valid arguments for this command are:
 *      - parv[0] = sender prefix
 *      - parv[1] = action [LOAD, UNLOAD, RELOAD, LIST]
 *      - parv[2] = module name
 */
static void
mo_module(struct Client *client_p, struct Client *source_p,
          int parc, char *parv[])
{
  const char *m_bn = NULL;
  struct module *modp = NULL;
  int check_core;

  if (!HasOFlag(source_p, OPER_FLAG_MODULE))
  {
    sendto_one(source_p, form_str(ERR_NOPRIVILEGES),
               me.name, source_p->name);
    return;
  }

  if (EmptyString(parv[1]))
  {
    sendto_one(source_p, form_str(ERR_NEEDMOREPARAMS),
               me.name, source_p->name, "MODULE");
    return;
  }

  if (!irccmp(parv[1], "LOAD"))
  {
    if (EmptyString(parv[2]))
    {
      sendto_one(source_p, form_str(ERR_NEEDMOREPARAMS),
                 me.name, source_p->name, "MODULE");
      return;
    }

    if (findmodule_byname((m_bn = libio_basename(parv[2]))) != NULL)
    {
      sendto_one(source_p, ":%s NOTICE %s :Module %s is already loaded",
                 me.name, source_p->name, m_bn);
      return;
    }

    load_one_module(parv[2]);
    return;
  }

  if (!irccmp(parv[1], "UNLOAD"))
  {
    if (EmptyString(parv[2]))
    {
      sendto_one(source_p, form_str(ERR_NEEDMOREPARAMS),
                 me.name, source_p->name, "MODULE");
      return;
    }

    if ((modp = findmodule_byname((m_bn = libio_basename(parv[2])))) == NULL)
    {
      sendto_one(source_p, ":%s NOTICE %s :Module %s is not loaded",
                 me.name, source_p->name, m_bn);
      return;
    }

    if (modp->flags & MODULE_FLAG_CORE)
    {
      sendto_one(source_p,
                 ":%s NOTICE %s :Module %s is a core module and may not be unloaded",
                 me.name, source_p->name, m_bn);
      return;
    }

    if (modp->flags & MODULE_FLAG_NOUNLOAD)
    {
      sendto_one(source_p,
                 ":%s NOTICE %s :Module %s is a resident module and may not be unloaded",
                 me.name, source_p->name, m_bn);
      return;
    }

    if (unload_one_module(m_bn, 1) == -1)
      sendto_one(source_p, ":%s NOTICE %s :Module %s is not loaded",
                 me.name, source_p->name, m_bn);
    return;
  }

  if (!irccmp(parv[1], "RELOAD"))
  {
    if (EmptyString(parv[2]))
    {
      sendto_one(source_p, form_str(ERR_NEEDMOREPARAMS),
                 me.name, source_p->name, "MODULE");
      return;
    }

    if (!strcmp(parv[2], "*"))
    {
      unsigned int modnum = 0;
      dlink_node *ptr = NULL, *ptr_next = NULL;

      sendto_one(source_p, ":%s NOTICE %s :Reloading all modules",
                 me.name, source_p->name);

      modnum = dlink_list_length(&modules_list);

      DLINK_FOREACH_SAFE(ptr, ptr_next, modules_list.head)
      {
        modp = ptr->data;

        if (!(modp->flags & MODULE_FLAG_NOUNLOAD))
          unload_one_module(modp->name, 0);
      }

      load_all_modules(0);
      load_conf_modules();
      load_core_modules(0);

      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Module Restart: %u modules unloaded, %u modules loaded",
                           modnum, dlink_list_length(&modules_list));
      ilog(LOG_TYPE_IRCD, "Module Restart: %u modules unloaded, %u modules loaded",
           modnum, dlink_list_length(&modules_list));
      return;
    }

    if ((modp = findmodule_byname((m_bn = libio_basename(parv[2])))) == NULL)
    {
      sendto_one(source_p, ":%s NOTICE %s :Module %s is not loaded",
               me.name, source_p->name, m_bn);
      return;
    }

    if (modp->flags & MODULE_FLAG_NOUNLOAD)
    {
      sendto_one(source_p,
                 ":%s NOTICE %s :Module %s is a resident module and may not be unloaded",
                 me.name, source_p->name, m_bn);
      return;
    }

    check_core = (modp->flags & MODULE_FLAG_CORE) != 0;

    if (unload_one_module(m_bn, 1) == -1)
    {
      sendto_one(source_p, ":%s NOTICE %s :Module %s is not loaded",
                 me.name, source_p->name, m_bn);
      return;
    }

    if ((load_one_module(parv[2]) == -1) && check_core)
    {
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Error reloading core "
                           "module: %s: terminating ircd", parv[2]);
      ilog(LOG_TYPE_IRCD, "Error loading core module %s: terminating ircd", parv[2]);
      exit(0);
    }

    return;
  }

  if (!irccmp(parv[1], "LIST"))
  {
    const dlink_node *ptr = NULL;

    DLINK_FOREACH(ptr, modules_list.head)
    {
      modp = ptr->data;

      if (!EmptyString(parv[2]) && match(parv[2], modp->name))
        continue;

      sendto_one(source_p, form_str(RPL_MODLIST), me.name, source_p->name,
                 modp->name, modp->handle,
                 modp->version, (modp->flags & MODULE_FLAG_CORE) ?"(core)":"");
    }

    sendto_one(source_p, form_str(RPL_ENDOFMODLIST),
               me.name, source_p->name);
    return;
  }

  sendto_one(source_p, ":%s NOTICE %s :%s is not a valid option. "
             "Choose from LOAD, UNLOAD, RELOAD, LIST",
             me.name, source_p->name, parv[1]);
}


static struct Message module_msgtab = {
 "MODULE", 0, 0, 2, MAXPARA, MFLG_SLOW, 0,
  {m_unregistered, m_not_oper, m_ignore, m_ignore, mo_module, m_ignore}
};

static void
module_init(void)
{
  mod_add_cmd(&module_msgtab);
}

static void
module_exit(void)
{
  mod_del_cmd(&module_msgtab);
}

struct module module_entry = {
  .node    = { NULL, NULL, NULL },
  .name    = NULL,
  .version = "$Revision: 1834 $",
  .handle  = NULL,
  .modinit = module_init,
  .modexit = module_exit,
  .flags   = MODULE_FLAG_NOUNLOAD
};
