/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  s_serv.h: A header for the server functions.
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
 *  $Id: s_serv.h 2294 2013-06-19 10:54:30Z michael $
 */

#ifndef INCLUDED_serv_h
#define INCLUDED_serv_h
#include "config.h"

struct MaskItem;

/*
 * number of seconds to wait after server starts up, before
 * starting try_connections()
 * TOO SOON and you can nick collide like crazy. 
 */
#define STARTUP_CONNECTIONS_TIME 60

struct Client;
struct MaskItem;
struct Channel;

/* Capabilities */
struct Capability
{
  dlink_node node;
  char *name;       /* name of capability */
  unsigned int cap; /* mask value         */
};

#define CAP_CAP         0x00000001 /* received a CAP to begin with        */
#define CAP_QS          0x00000002 /* Can handle quit storm removal       */
#define CAP_EX          0x00000004 /* Can do channel +e exemptions        */
#define CAP_CHW         0x00000008 /* Can do channel wall @#              */
#define CAP_IE          0x00000010 /* Can do invite exceptions            */
#define CAP_EOB         0x00000020 /* Can do EOB message                  */
#define CAP_KLN         0x00000040 /* Can do KLINE message                */
#define CAP_GLN         0x00000080 /* Can do GLINE message                */
#define CAP_TS6         0x00000100 /* Can do TS6                          */
#define CAP_KNOCK       0x00000200 /* supports KNOCK                      */
#define CAP_UNKLN       0x00000400 /* Can do UNKLINE message              */
#define CAP_CLUSTER     0x00000800 /* supports server clustering          */
#define CAP_ENCAP       0x00001000 /* supports ENCAP message              */
#define CAP_HOPS        0x00002000 /* supports HALFOPS                    */
#define CAP_TBURST      0x00004000 /* supports TBURST                     */
#define CAP_SVS         0x00008000 /* supports services                   */
#define CAP_DLN         0x00010000 /* Can do DLINE message                */
#define CAP_UNDLN       0x00020000 /* Can do UNDLINE message              */
#define CAP_FAKEHOST    0x00040000 /* supports fake hosts                 */

/*
 * Capability macros.
 */
#define IsCapable(x, cap)       ((x)->localClient->caps &   (cap))
#define SetCapable(x, cap)      ((x)->localClient->caps |=  (cap))
#define ClearCap(x, cap)        ((x)->localClient->caps &= ~(cap))


/*
 * return values for hunt_server() 
 */
#define HUNTED_NOSUCH   (-1)    /* if the hunted server is not found */
#define HUNTED_ISME     0       /* if this server should execute the command */
#define HUNTED_PASS     1       /* if message passed onwards successfully */

extern int valid_servname(const char *);
extern int check_server(const char *, struct Client *);
extern int hunt_server(struct Client *, struct Client *,
                       const char *, const int, const int, char *[]);
extern void add_capability(const char *, int, int);
extern int delete_capability(const char *);
extern int unsigned find_capability(const char *);
extern void send_capabilities(struct Client *, int);
extern void write_links_file(void *);
extern void read_links_file(void);
extern void server_estab(struct Client *);
extern const char *show_capabilities(const struct Client *);
extern void try_connections(void *);
extern void burst_channel(struct Client *client_p, struct Channel *);
extern void sendnick_TS(struct Client *, struct Client *);
extern int serv_connect(struct MaskItem *, struct Client *);
extern struct Client *find_servconn_in_progress(const char *);
extern struct Server *make_server(struct Client *);
#endif /* INCLUDED_s_serv_h */
