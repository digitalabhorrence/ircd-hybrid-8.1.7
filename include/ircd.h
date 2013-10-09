/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  ircd.h: A header for the ircd startup routines.
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
 *  $Id: ircd.h 1858 2013-04-25 15:00:52Z michael $
 */

#ifndef INCLUDED_ircd_h
#define INCLUDED_ircd_h

#include "ircd_defs.h"
#include "config.h"


struct SetOptions
{
  int autoconn;      /* autoconn enabled for all servers? */
  int floodcount;    /* Number of messages in 1 second    */
  int joinfloodtime;
  int joinfloodcount;
  int ident_timeout; /* timeout for identd lookups        */
  int spam_num;
  int spam_time;
};

/*
 * statistics structures
 */
struct ServerStatistics
{
  uint64_t        is_cbs;  /* bytes sent to clients */
  uint64_t        is_cbr;  /* bytes received from clients */
  uint64_t        is_sbs;  /* bytes sent to servers */
  uint64_t        is_sbr;  /* bytes received from servers */

  time_t          is_cti;  /* time spent connected by clients */
  time_t          is_sti;  /* time spent connected by servers */

  unsigned int    is_cl;   /* number of client connections */
  unsigned int    is_sv;   /* number of server connections */
  unsigned int    is_ni;   /* connection but no idea who it was */
  unsigned int    is_ac;   /* connections accepted */
  unsigned int    is_ref;  /* accepts refused */
  unsigned int    is_unco; /* unknown commands */
  unsigned int    is_wrdi; /* command going in wrong direction */
  unsigned int    is_unpf; /* unknown prefix */
  unsigned int    is_empt; /* empty message */
  unsigned int    is_num;  /* numeric message */
  unsigned int    is_kill; /* number of kills generated on collisions */
  unsigned int    is_asuc; /* successful auth requests */
  unsigned int    is_abad; /* bad auth requests */
};

struct Counter
{
  uint64_t totalrestartcount; /* Total client count ever */
  unsigned int myserver;      /* my servers          */
  unsigned int oper;          /* Opers               */
  unsigned int local;         /* Local Clients       */
  unsigned int total;         /* total clients       */
  unsigned int invisi;        /* invisible clients   */
  unsigned int max_loc;       /* MAX local clients   */
  unsigned int max_tot;       /* MAX global clients  */
  unsigned int max_loc_con;   /* MAX local connection count (clients + server) */
  unsigned int max_loc_cli;   /* XXX This is redundant - Max local client count */
};

struct ServerState_t
{
  int foreground;
};


#ifdef HAVE_LIBGEOIP
extern GeoIP *geoip_ctx;
#endif
extern char **myargv;
extern const char *infotext[];
extern const char *serno;
extern const char *ircd_version;
extern const char *logFileName;
extern const char *pidFileName;
extern int dorehash;
extern int doremotd;
extern struct Counter Count;
extern struct ServerStatistics ServerStats;
extern struct SetOptions GlobalSetOptions; /* defined in ircd.c */
extern struct ServerState_t server_state;
extern struct timeval SystemTime;
#define CurrentTime SystemTime.tv_sec
extern int default_server_capabs;
extern unsigned int splitmode;
extern unsigned int splitchecking;
extern unsigned int split_users;
extern unsigned int split_servers;

extern int rehashed_klines;
extern void set_time(void);
#endif
