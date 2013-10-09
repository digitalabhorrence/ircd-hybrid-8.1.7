/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  numeric.h: A header for the numeric functions.
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
 *  $Id: numeric.h 2237 2013-06-15 19:17:25Z michael $
 */

#ifndef INCLUDED_numeric_h
#define INCLUDED_numeric_h

/* form_str - return a format string for a message number
 * messages are defined below
 */
extern const char *form_str(unsigned int);

/*
 * Reserve numerics 000-099 for server-client connections where the client
 * is local to the server. If any server is passed a numeric in this range
 * from another server then it is remapped to 100-199. -avalon
 */
#define RPL_WELCOME          001
#define RPL_YOURHOST         002
#define RPL_CREATED          003
#define RPL_MYINFO           004
#define RPL_ISUPPORT         005
#define RPL_REDIR            10
#define RPL_MAP		     15		/* Undernet extension */
#define RPL_MAPMORE	     16		/* Undernet extension */
#define RPL_MAPEND	     17		/* Undernet extension */

#define RPL_YOURID	     42		/* IRCnet extension */

/*
 * Numeric replies from server commands.
 * These are currently in the range 200-399.
 */
#define RPL_TRACELINK        200
#define RPL_TRACECONNECTING  201
#define RPL_TRACEHANDSHAKE   202
#define RPL_TRACEUNKNOWN     203
#define RPL_TRACEOPERATOR    204
#define RPL_TRACEUSER        205
#define RPL_TRACESERVER      206
#define RPL_TRACENEWTYPE     208
#define RPL_TRACECLASS       209
#define RPL_STATSLINKINFO    211
#define RPL_STATSCOMMANDS    212
#define RPL_STATSCLINE       213
#define RPL_STATSNLINE       214
#define RPL_STATSILINE       215
#define RPL_STATSKLINE       216
#define RPL_STATSQLINE       217
#define RPL_STATSYLINE       218
#define RPL_ENDOFSTATS       219

/*
 * note ircu uses 217 for STATSPLINE frip. conflict
 * as RPL_STATSQLINE was used in old 2.8 for Q line 
 * I'm going to steal 220 for now *sigh*
 * -Dianora
 */

#define RPL_STATSPLINE       220
#define RPL_UMODEIS          221
#define RPL_STATSFLINE       224
#define RPL_STATSDLINE       225
#define RPL_STATSALINE	     226
#define RPL_STATSLLINE       241
#define RPL_STATSUPTIME      242
#define RPL_STATSOLINE       243
#define RPL_STATSHLINE       244
#define RPL_STATSTLINE       245
#define RPL_STATSSERVICE     246
#define RPL_STATSXLINE       247
#define RPL_STATSULINE       248
#define RPL_STATSDEBUG       249
#define RPL_STATSCONN        250
#define RPL_LUSERCLIENT      251
#define RPL_LUSEROP          252
#define RPL_LUSERUNKNOWN     253
#define RPL_LUSERCHANNELS    254
#define RPL_LUSERME          255
#define RPL_ADMINME          256
#define RPL_ADMINLOC1        257
#define RPL_ADMINLOC2        258
#define RPL_ADMINEMAIL       259
#define RPL_ENDOFTRACE       262
#define RPL_LOAD2HI          263
#define RPL_LOCALUSERS       265
#define RPL_GLOBALUSERS      266
#define RPL_WHOISCERTFP      276
#define RPL_ACCEPTLIST	     281
#define RPL_ENDOFACCEPT      282
#define RPL_NEWHOSTIS        285

/* numeric_replies */
#define RPL_AWAY             301
#define RPL_USERHOST         302
#define RPL_ISON             303
#define RPL_UNAWAY           305
#define RPL_NOWAWAY          306
#define RPL_WHOISREGNICK     307
#define RPL_WHOISADMIN	     308

#define RPL_WHOISUSER        311
#define RPL_WHOISSERVER      312
#define RPL_WHOISOPERATOR    313

#define RPL_WHOWASUSER       314
#define RPL_ENDOFWHO         315
#define RPL_WHOISCHANOP      316 /* redundant and not needed but reserved */
#define RPL_WHOISIDLE        317
#define RPL_ENDOFWHOIS       318
#define RPL_WHOISCHANNELS    319
#define RPL_LISTSTART        321
#define RPL_LIST             322
#define RPL_LISTEND          323
#define RPL_CHANNELMODEIS    324
#define RPL_CREATIONTIME     329
#define RPL_NOTOPIC          331
#define RPL_TOPIC            332
#define RPL_TOPICWHOTIME     333
#define	RPL_WHOISACTUALLY    338
#define RPL_INVITING         341
#define RPL_INVITELIST       346
#define RPL_ENDOFINVITELIST  347
#define RPL_EXCEPTLIST       348
#define RPL_ENDOFEXCEPTLIST  349
#define RPL_VERSION          351
#define RPL_WHOREPLY         352
#define RPL_NAMREPLY         353
#define RPL_CLOSING          362
#define RPL_CLOSEEND         363
#define RPL_LINKS            364
#define RPL_ENDOFLINKS       365
#define RPL_ENDOFNAMES       366
#define RPL_BANLIST          367
#define RPL_ENDOFBANLIST     368
#define RPL_ENDOFWHOWAS      369
#define RPL_INFO             371
#define RPL_MOTD             372
#define RPL_INFOSTART        373
#define RPL_ENDOFINFO        374
#define RPL_MOTDSTART        375
#define RPL_ENDOFMOTD        376
#define RPL_YOUREOPER        381
#define RPL_REHASHING        382
#define RPL_RSACHALLENGE     386
#define RPL_TIME             391
#define RPL_USERSSTART       392
#define RPL_USERS            393
#define RPL_ENDOFUSERS       394
#define RPL_NOUSERS          395
#define RPL_HOSTHIDDEN       396

/*
 * Errors are in the range from 400-599 currently and are grouped by what
 * commands they come from.
 */
#define ERR_NOSUCHNICK       401
#define ERR_NOSUCHSERVER     402
#define ERR_NOSUCHCHANNEL    403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_TOOMANYCHANNELS  405
#define ERR_WASNOSUCHNICK    406
#define ERR_TOOMANYTARGETS   407
#define ERR_NOCTRLSONCHAN    408
#define ERR_NOORIGIN         409
#define ERR_INVALIDCAPCMD    410

#define ERR_NORECIPIENT      411
#define ERR_NOTEXTTOSEND     412
#define ERR_NOTOPLEVEL       413
#define ERR_WILDTOPLEVEL     414
#define ERR_UNKNOWNCOMMAND   421
#define ERR_NOMOTD           422
#define ERR_NOADMININFO      423
#define ERR_NONICKNAMEGIVEN  431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE    433
#define ERR_NICKCOLLISION    436
#define ERR_UNAVAILRESOURCE  437
#define ERR_NICKTOOFAST	     438	/* We did it first Undernet! ;) db */
#define ERR_SERVICESDOWN     440
#define ERR_USERNOTINCHANNEL 441
#define ERR_NOTONCHANNEL     442
#define ERR_USERONCHANNEL    443
#define ERR_NOTREGISTERED    451
#define ERR_ACCEPTFULL       456
#define ERR_ACCEPTEXIST      457
#define ERR_ACCEPTNOT        458
#define ERR_NEEDMOREPARAMS   461
#define ERR_ALREADYREGISTRED 462
#define ERR_PASSWDMISMATCH   464
#define ERR_YOUREBANNEDCREEP 465
#define ERR_ONLYSERVERSCANCHANGE 468
#define ERR_OPERONLYCHAN     470
#define ERR_CHANNELISFULL    471
#define ERR_UNKNOWNMODE      472
#define ERR_INVITEONLYCHAN   473
#define ERR_BANNEDFROMCHAN   474
#define ERR_BADCHANNELKEY    475
#define ERR_NEEDREGGEDNICK   477
#define ERR_BANLISTFULL      478        /* I stole the numeric from ircu -db */
#define ERR_BADCHANNAME      479
#define ERR_SSLONLYCHAN      480
#define ERR_NOPRIVILEGES     481
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_CANTKILLSERVER   483
#define ERR_RESTRICTED       484
#define ERR_CHANBANREASON    485
#define ERR_NONONREG         486
#define ERR_NOOPERHOST       491
#define ERR_UMODEUNKNOWNFLAG 501
#define ERR_USERSDONTMATCH   502
#define ERR_GHOSTEDCLIENT    503
#define ERR_USERNOTONSERV    504
#define ERR_TOOMANYWATCH     512
#define ERR_WRONGPONG	     513
#define ERR_LONGMASK	     518	/* Undernet extension -Kev */
#define	ERR_LISTSYNTAX       521
#define ERR_HELPNOTFOUND     524
#define RPL_LOGON            600
#define RPL_LOGOFF           601
#define RPL_WATCHOFF         602
#define RPL_WATCHSTAT        603
#define RPL_NOWON            604
#define RPL_NOWOFF           605
#define RPL_WATCHLIST        606
#define RPL_ENDOFWATCHLIST   607
#define RPL_WHOISSECURE      671
#define RPL_MODLIST          702
#define RPL_ENDOFMODLIST     703
#define RPL_HELPSTART        704
#define RPL_HELPTXT          705
#define RPL_ENDOFHELP        706
#define RPL_ETRACE           709
#define RPL_KNOCK	     710
#define RPL_KNOCKDLVR	     711
#define ERR_TOOMANYKNOCK     712
#define ERR_CHANOPEN         713
#define ERR_KNOCKONCHAN      714
#define RPL_TARGUMODEG       716
#define RPL_TARGNOTIFY       717
#define RPL_UMODEGMSG        718
#define ERR_NOPRIVS	     723

#define ERR_LAST_ERR_MSG     999

#endif /* INCLUDED_numeric_h */
