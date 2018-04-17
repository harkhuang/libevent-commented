/*
 * Copyright 2000 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Niels Provos.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _EVENT_H_
#define _EVENT_H_

#define EVLIST_TIMEOUT	0x01
#define EVLIST_READ	0x02
#define EVLIST_WRITE	0x04
#define EVLIST_ADD	0x08
#define EVLIST_INIT	0x80

#define EV_TIMEOUT	EVLIST_TIMEOUT
#define EV_READ		EVLIST_READ
#define EV_WRITE	EVLIST_WRITE

struct event {
  struct {struct event *tqe_next; struct event **tqe_prev;}  ev_read_next1;

	TAILQ_ENTRY (event) ev_read_next; //ÉùÃ÷event½á¹¹Ìå Ë«¶Ë¶ÓÁÐµÄ¹Ø¼üÖ¸Õë  ¶ÓÁÐÊÇ´æÔÚÕ»ÖÐ
	TAILQ_ENTRY (event) ev_write_next; //ÉùÃ÷event½á¹¹Ìå  Ë«¶Ë¶ÓÁÐ¹Ø¼üÖ¸Õë  Õ»ÖÐµÄË«¶Ë¶ÓÁÐÍ·
	TAILQ_ENTRY (event) ev_timeout_next;
	TAILQ_ENTRY (event) ev_add_next;

	int ev_fd;   //Ã¿¸öevent °ó¶¨Ò»¸öÎÄ¼þÃèÊö·û
	short ev_events;  // ±íÊ¾event¸öÊýå? ±àºÅå?

	struct timeval ev_timeout;  // ÄÚºËÊ±¼äÖµ

  // Ò»¿Å»Øµ÷º¯Êý  °üº¬ÁËÈý¸ö²ÎÊý  ÆäÖÐÒ»¸öÊÇ´«ÈëµÄ²ûÊö  Á½¸ö´©³öµÄ²ÎÊýå?å?
	void (*ev_callback)(int, short, void *arg); 
	void *ev_arg; // »Øµ÷º¯ÊýµÄ²ÎÊý

	int ev_flags; // ±êÖ¾Î»å?  ²»¶®ÒâÍ¼å?
};

#define TIMEOUT_DEFAULT	5   // Ä¬ÈÏ»Øµ÷º¯Êý5ÃëºóÖ´ÐÐ

void event_init(void);// ³õÊ¼»¯ÊÂ¼þå?  ³õÊ¼»¯ÊÂ¼þ¶ÓÁÐå?ÉêÇë¿Õ¼ä ·ÅÈëÄÚ´æå?

int event_dispatch(void); //Ê±¼ä·Ö·¢å?´¦Àíå? ÕâÀïÊÇÖ´ÐÐ»Øµ÷º¯Êýå?ºÍ´¦Àí·½Ê½

int timeout_next(struct timeval *); //å?å?å?
void timeout_process(void);//å?å?


// Ìí¼ÓÊÂ¼þ
#define timeout_add(ev, tv)		event_add(ev, tv)


// ÉèÖÃÊ±¼ä
#define timeout_set(ev, cb, arg)	event_set(ev, -1, 0, cb, arg)

// É¾³ýÊÂ¼þ
#define timeout_del(ev)			event_del(ev)

// ÊÂ¼þÎ´¾ö
#define timeout_pending(ev, tv)		event_pending(ev, EV_TIMEOUT, tv)

// ÊÂ¼þ³õÊ¼»¯
#define timeout_initalized(ev)		((ev)->ev_flags & EVLIST_INIT)

void event_set(struct event *, int, short, void (*)(int, short, void *), void *);
void event_add(struct event *, struct timeval *);
void event_del(struct event *);

int event_pending(struct event *, short, struct timeval *);

#define event_initalized(ev)		((ev)->ev_flags & EVLIST_INIT)

#endif /* _EVENT_H_ */
