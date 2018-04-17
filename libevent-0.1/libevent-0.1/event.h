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

	TAILQ_ENTRY (event) ev_read_next; //����event�ṹ�� ˫�˶��еĹؼ�ָ��  �����Ǵ���ջ��
	TAILQ_ENTRY (event) ev_write_next; //����event�ṹ��  ˫�˶��йؼ�ָ��  ջ�е�˫�˶���ͷ
	TAILQ_ENTRY (event) ev_timeout_next;
	TAILQ_ENTRY (event) ev_add_next;

	int ev_fd;   //ÿ��event ��һ���ļ�������
	short ev_events;  // ��ʾevent�����? ����?

	struct timeval ev_timeout;  // �ں�ʱ��ֵ

  // һ�Żص�����  ��������������  ����һ���Ǵ���Ĳ���  ���������Ĳ����?�?
	void (*ev_callback)(int, short, void *arg); 
	void *ev_arg; // �ص������Ĳ���

	int ev_flags; // ��־λ�?  ������ͼ�?
};

#define TIMEOUT_DEFAULT	5   // Ĭ�ϻص�����5���ִ��

void event_init(void);// ��ʼ���¼��?  ��ʼ���¼������?����ռ� �����ڴ��?

int event_dispatch(void); //ʱ��ַ��?�����? ������ִ�лص������?�ʹ���ʽ

int timeout_next(struct timeval *); //�?�?�?
void timeout_process(void);//�?�?


// ����¼�
#define timeout_add(ev, tv)		event_add(ev, tv)


// ����ʱ��
#define timeout_set(ev, cb, arg)	event_set(ev, -1, 0, cb, arg)

// ɾ���¼�
#define timeout_del(ev)			event_del(ev)

// �¼�δ��
#define timeout_pending(ev, tv)		event_pending(ev, EV_TIMEOUT, tv)

// �¼���ʼ��
#define timeout_initalized(ev)		((ev)->ev_flags & EVLIST_INIT)

void event_set(struct event *, int, short, void (*)(int, short, void *), void *);
void event_add(struct event *, struct timeval *);
void event_del(struct event *);

int event_pending(struct event *, short, struct timeval *);

#define event_initalized(ev)		((ev)->ev_flags & EVLIST_INIT)

#endif /* _EVENT_H_ */
