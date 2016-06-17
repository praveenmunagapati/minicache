/*
 * Double linked list implementation
 *
 * Copyright(C) 2014 NEC Laboratories Europe. All rights reserved.
 *                   Simon Kuenzer <simon.kuenzer@neclab.eu>
 */
#ifndef _DLIST_H_
#define _DLIST_H_

#include <stdint.h>
#include <errno.h>
#include "likely.h"

struct dlist_el {
	void *next;
	void *prev;
};
#define dlist_el(dlname) \
	struct dlist_el (dlname)

struct dlist_head {
	void *first;
	void *last;
};
#define dlist_head(dlname) \
	struct dlist_head (dlname)

#define dlist_init_head(head)\
	do { \
		(head).first = NULL; \
		(head).last = NULL; \
	} while (0)
#define dlist_init_el(el, dlname) /* optional as long as */	  \
                                  /* dlist_is_linked() is not used  */	  \
	do { \
		(el)->dlname.next = NULL; \
		(el)->dlname.prev = NULL; \
	} while (0)

#define dlist_is_empty(head) \
	((head).first == NULL)

#define dlist_next_el(el, dlname) \
	((typeof((el))) (el)->dlname.next)
#define dlist_prev_el(el, dlname) \
	((typeof((el))) (el)->dlname.prev)
#define dlist_first_el(head, eltype) \
	((eltype *) ((head).first))
#define dlist_last_el(head, eltype) \
	((eltype *) ((head).last))

/* Note: This function only supports checking if an element is linked into
 *       a specific list. It will fail as soon as the element is linked to a
 *       different list (with same dlname) than checked against */
#define dlist_is_linked(el, head, dlname) /* requires dlist_init_el() */ \
	(((el)->dlname.prev != NULL || (el)->dlname.next != NULL) || \
	 ((head).first == (el) || (head).last == (el)))

#define dlist_unlink(el, head, dlname) \
	do { \
		/* element part of this list? (brief check,
		 * works only if element is at the beginning
		 * or at the end of the list) */ \
		BUG_ON(!(el)->dlname.prev && (head).first != (el)); \
		BUG_ON(!(el)->dlname.next && (head).last != (el)); \
		\
		if ((el)->dlname.prev) { \
			((typeof((el))) (el)->dlname.prev)->dlname.next = (el)->dlname.next; \
		} else {  \
			(head).first = (el)->dlname.next; \
		} \
		if ((el)->dlname.next) {  \
			((typeof((el))) (el)->dlname.next)->dlname.prev = (el)->dlname.prev; \
		} else {  \
			(head).last = (el)->dlname.prev; \
		} \
		dlist_init_el((el), dlname); \
	} while(0)

#define dlist_foreach(el, head, dlname) \
	for((el) = ((typeof((el))) (head).first); \
	    (el) != NULL; \
	    (el) = dlist_next_el((el), dlname))

#define dlist_foreach_reverse(el, head, dlname) \
	for((el) = ((typeof((el))) (head).last); \
	    (el) != NULL; \
	    (el) = dlist_prev_el((el), dlname))

#define dlist_append(el, head, dlname) \
	do { \
		if (dlist_is_empty((head))) { \
			(head).first = (el); \
			(el)->dlname.prev = NULL; \
		} else { \
			((typeof((el))) (head).last)->dlname.next = (el); \
			(el)->dlname.prev = (head).last; \
		} \
		(el)->dlname.next = NULL; \
		(head).last = (el); \
	} while(0)
#define dlist_relink_tail(el, head, dlname) \
	do { \
		dlist_unlink((el), (head), dlname); \
		dlist_append((el), (head), dlname); \
	} while(0)

#define dlist_prepend(el, head, dlname) \
	do { \
		if (dlist_is_empty((head))) { \
			(head).last = (el); \
			(el)->dlname.next = NULL; \
		} else { \
			((typeof((el))) (head).first)->dlname.prev = (el); \
			(el)->dlname.next = (head).first; \
		} \
		(el)->dlname.prev = NULL; \
		(head).first = (el); \
	} while(0)
#define dlist_relink_head(el, head, dlname) \
	do { \
		dlist_unlink((el), (head), dlname); \
		dlist_prepend((el), (head), dlname); \
	} while(0)

#endif /* _DLIST_H_ */
