/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  memory.h: A header for the memory functions.
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
 *  $Id: memory.h 1664 2012-11-18 14:33:47Z michael $
 */

#ifndef _I_MEMORY_H
#define _I_MEMORY_H

#include "ircd_defs.h"

extern void outofmemory(void);

extern void *MyMalloc(size_t);
extern void *MyRealloc(void *, size_t);
extern void MyFree(void *);
extern void *xstrdup(const char *);
extern void *xstrndup(const char *, size_t);
#endif /* _I_MEMORY_H */
