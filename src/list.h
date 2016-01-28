#ifndef _LIST_H_
#define _LIST_H_

typedef struct list_t 
{
	struct list_t* next;
	void* value;
} list_t;

list_t* list_create(void* data);
list_t* list_node_at(list_t* head, int id);
void* list_get(list_t* head, int id);
void list_free(list_t** head);
int list_length(list_t* head);
void list_add_front(list_t** head, void* data);
void list_add_back(list_t** head, void* data);
void list_remove(list_t** head, void* data);
void* list_shift(list_t** head);
#endif
