#ifndef __LIST_H__
#define __LIST_H__

typedef struct list_element {
    int value;
    struct list_element *prev;
    struct list_element *next;
} list_element;

typedef struct list {
    struct list_element *head;
    struct list_element *tail;
    int size;
    int middle;
} list;

void init_list(struct list *l);
void add_element(struct list *l, int value);
int set_element(struct list *l, int index, int value);
void remove_element(struct list *l, int value);
int get_element(struct list *l, int index);
int get_position(struct list *l, int value);
int list_size(struct list *l);
void delete_list(struct list *l);
void print_list(struct list *l);

#endif