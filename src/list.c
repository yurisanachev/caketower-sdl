#include <stdlib.h>
#include "list.h"


list_t* list_create(void* data)
{
	list_t* temp = (list_t*)malloc(sizeof(list_t));
	temp->value = data;
	temp->next = NULL;
	return temp;
}


void list_remove(list_t** head, void* elem)
{
	int i = -1;	
	int total = 0;

	list_t* temp = *head;
	list_t* temp2;

	// search for the item id we're gonna delete
	while (temp)
	{
		if (temp->value == elem) i = total;
		temp = temp->next;
		total++;
	}

	if (i == -1) 
	{
		return;
	}
	
	if (i == 0)
	{
		// remove head
		if (total == 1)
		{
			list_free(head);
		} else {
			// (a)-(b)-(c)
			temp = (*head);
			(*head) = (*head)->next;
			
			free(temp);
		} 
	} else if ((i + 1) == total)
	{
		temp = list_node_at(*head, i - 1);
		free(temp->next);
		temp->next = NULL;		
	} else {
		// divide this shit
		temp = list_node_at(*head, i - 1);
		temp2 = temp->next;
		temp->next = temp->next->next;
		
		free(temp2);	
	}
}

list_t* list_node_at(list_t* head, int id)
{
	int i = 0; 	

	while (i != id)
	{
		i++;
		if (head == NULL) break;
		head = head->next;
	}

	return head;
}

void* list_get(list_t* head, int id)
{
	list_t* n = list_node_at(head, id);
	if (n == NULL) return 0;
	return n->value;
}

void list_free(list_t** head)
{
	list_t* it = *head;
	while (it)
	{
		list_t* temp = it->next;
		free(it);
		it = temp;
	}
	*head = NULL;
}

int list_length(list_t* head)
{
	int c = 0;
	while (head)
	{
		head = head->next;
		c++;
	}
	return c;
}

void list_add_front(list_t** head, void* data) 
{
	list_t* temp = list_create(data);
	temp->next = (*head);
	(*head) = temp;		
}

void list_add_back(list_t** head, void* data) 
{
	int l = list_length(*head);
	list_t* temp;
	if (l == 0) 
	{
		list_add_front(head, data);
		return;
	}
	temp = list_create(data);
	list_node_at(*head, l - 1)->next = temp;
}
