#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    ptr = q->head;
    while (ptr) {
        list_ele_t *self;
        self = ptr;
        ptr = ptr->next;
        if (self->value) {
            free(self->value);
        }
        free(self);
    }
    free(q);
    return 0;
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{   
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh || !q) {
        return false;
    }
    
    newh->value = strdup(s);
    if (!newh->value) {
        free(newh);
        return false;
    }

    newh->next = q->head;
    q->head = newh;
    if (!q->tail) {
        q->tail = q->head;
	}
	q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt || !q) {
        return false;
    }
    
    newt->value = strdup(s);
    if (!newt->value) {
        free(newt);
        return false;
    }
    
    if (!q->head){
        q->head = newt;
        q->tail = newt;
    }
    q->tail->next = newt;
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    list_ele_t *rmhead;
    if (!q || !q->head)
        return false;

    if (bufsize > 0 && sp) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    q->head = rmhead->next;
    q->nr--;
    if (rmhead == q->tail)
        q->tail = NULL;
    free(rmhead);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->nr;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }
    
    list_ele_t *pre, *cur, *nex;
    cur = q->head;
    pre = NULL;
    while (cur->next) {
        nex = cur-next;
        nex->next = pre;
        pre = cur;
        cur = nex;
    }
    cur->next = pre;
    q->tail = q->head;
    q->head = cur;
    return;
    
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size < 2) {
        return;
    }
    
    list_ele_t *ptr = q->head;
	list_ele_t *smallest = ptr;
	list_ele_t *prev = ptr;
	list_ele_t *sorted;
	list_ele_t *unsorted = ptr;
    char tmp = ptr->value;
    while (ptr) {
    	if (tmp > ptr->next->value) {
    		tmp = ptr->next->value;
    		prev = ptr;
    		smallest = ptr->next;
    	}
    }
    if (q->head == smallest) {
    	unsorted = q->head->next;
    }
    q->head = smallest;
    sorted = smallest;
    
    if (prev != smallest) {
    	prev->next = smallest->next;
    }
	
    for (int i = q->size; i > 1; i--) {
    	ptr = unsorted;
    	tmp = ptr->value;
    	prev = ptr;
    	while (ptr) {
    		if (tmp > ptr->next->value) {
    			tmp = ptr->next->value;
    			prev = ptr;
    			smallest = ptr->next;
    		}
    		ptr = ptr->next;
    	}
    	if (prev != smallest) {
    		prev->next = smallest->next;
    	}
    	if (unsorted == smallest) {
    		unsorted = unsorted->next;
    	}
    	sorted->next = smallest;
    }
    smallest->next = NULL;
    q->tail = smallest;
}
