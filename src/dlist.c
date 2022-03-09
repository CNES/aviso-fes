/* This file is part of FES library.

   FES is free software: you can redistribute it and/or modify
   it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   FES is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU LESSER GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
   along with FES.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dlist.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void
dlist_init(fes_dlist* list, void (*del)(void* p))
{
  memset(list, 0, sizeof(fes_dlist));
  list->del = del;
}

void
dlist_destroy(fes_dlist* list)
{
  while (list->size) {
    void* data;

    if (dlist_remove(list, list->head, &data) == 0 && list->del) {
      list->del(data);
    }
  }
  memset(list, 0, sizeof(fes_dlist));
}

int
dlist_remove(fes_dlist* list, fes_dlist_item* item, void** data)
{
  if (item == NULL || list->size == 0)
    return -1;

  // Memorize deleted data
  *data = item->data;

  // Remove the head of the list
  if (item == list->head) {
    list->head = item->next;

    // The list is not empty ?
    if (list->head)
      item->next->prev = NULL;
    else
      list->tail = NULL;
  } else {
    // Remove somewhere else
    item->prev->next = item->next;

    // If the item has a successor
    if (item->next)
      item->next->prev = item->prev;
    else
      list->tail = item->prev;
  }

  // frees the storage allocated by the abstract type
  free(item);

  // Adjust the size of the list
  --(list->size);
  return 0;
}

static int
_ins_prev(fes_dlist* list, fes_dlist_item* item, const void* data)
{
  fes_dlist_item* new_item;

  // Do not accept a null item unless the list is empty.
  if (item == NULL && list->size) {
    return -1;
  }

  // Allocate memory for the item
  if ((new_item = (fes_dlist_item*)malloc(sizeof(fes_dlist_item))) == NULL)
    return -1;

  new_item->data = (void*)data;

  // Insert in an empty list
  if (list->size == 0) {
    list->head = list->tail = new_item;
    list->head->prev = NULL;
    list->head->next = NULL;
  } else {
    new_item->next = item;
    new_item->prev = item->prev;

    // Insert before the head of the list
    if (item->prev == NULL)
      list->head = new_item;
    else
      item->prev->next = new_item;

    item->prev = new_item;
  }

  // Adjust size of the list
  ++(list->size);
  return 0;
}

int
dlist_move_front(fes_dlist* list, fes_dlist_item* item)
{
  if (list == NULL || list->size == 0 || item == NULL)
    return -1;

  if (list->size == 1 || list->head == item)
    return 0;

  // Remove the item
  item->prev->next = item->next;
  if (item->next)
    item->next->prev = item->prev;
  else
    list->tail = item->prev;

  // Update the head properties
  item->prev = NULL;
  item->next = list->head;
  list->head->prev = item;
  list->head = item;

  return 0;
}

int
dlist_push_front(fes_dlist* list, const void* data)
{
  return _ins_prev(list, list->head, data);
}

int
dlist_pop_back(fes_dlist* list, void** data)
{
  return dlist_remove(list, list->tail, data);
}

size_t
dlist_size(const fes_dlist* const list)
{
  return list->size;
}

fes_dlist_item*
dlist_head(const fes_dlist* const list)
{
  return list->head;
}
