#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"
#include "strnatcmp.h"

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
    if (!q) {
        return;
    }
    list_ele_t *ptr = q->head;
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
    if (!q) {
        return false;
    }
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }
    newh->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    memcpy(newh->value, s, (strlen(s) + 1));
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
    if (!q) {
        return false;
    }
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        return false;
    }
    newt->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!newt->value) {
        free(newt);
        return false;
    }
    memcpy(newt->value, s, (strlen(s) + 1));
    if (!q->tail) {
        q->head = newt;
        q->tail = newt;
    } else {
        q->tail->next = newt;
        q->tail = newt;
    }
    newt->next = NULL;
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
    if (!q || !q->head)
        return false;

    list_ele_t *rmhead = q->head;

    if (bufsize > 0 && sp) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    q->head = rmhead->next;
    if (rmhead == q->tail) {
        q->tail = NULL;
    }
    free(rmhead->value);
    free(rmhead);
    q->size--;
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
    return q->size;
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
    if (!q || q->size < 2) {
        return;
    }
    list_ele_t *pre, *cur, *nex;
    pre = NULL;
    cur = q->head;
    nex = cur->next;
    q->tail = cur;
    while (nex) {
        cur->next = pre;
        pre = cur;
        cur = nex;
        nex = cur->next;
    }
    cur->next = pre;
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
    if (!q || q->size < 2)
        return;
    queue_t left, right;
    left.size = (q->size >> 1) + (q->size & 1);
    right.size = q->size >> 1;
    list_ele_t *cur = left.head = q->head;
    right.tail = q->tail;

    for (size_t i = 0; i < left.size - 1; i++)
        cur = cur->next;

    left.tail = cur;
    right.head = cur->next;
    left.tail->next = right.tail->next = NULL;
    q->head = q->tail = NULL;

    q_sort(&left);
    q_sort(&right);
    q_merge(&left, &right, q);
}

void q_merge(queue_t *left, queue_t *right, queue_t *q)
{
    q->size = left->size + right->size;
    list_ele_t *l = left->head, *r = right->head;
    list_ele_t *tem = NULL;
    if (strnatcmp(left->head->value, right->head->value) < 0) {
        q->head = left->head;
    } else {
        q->head = right->head;
    }
    q->tail = q->head;
    for (size_t i = 0; i < q->size; i++) {
        if (!r || (l && strnatcmp(l->value, r->value) < 0)) {
            tem = l;
            l = l->next;
        } else {
            tem = r;
            r = r->next;
        }
        q->tail->next = tem;
        q->tail = tem;
    }
    tem->next = NULL;
}