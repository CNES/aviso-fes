/**
 @file dlist.h
 @brief Handle a double linked list
 @author CLS
 */

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

#pragma once
#include "fes_int.h"

/**
 @brief List initialization

 @param list List handler
 @param del Function to call during the destruction of the list
 */
void
dlist_init(fes_dlist* list, void (*del)(void* p));

/**
 @brief Remove all items from the list

 @param list List handler
 */
void
dlist_destroy(fes_dlist* list);

/**
 @brief Remove an item from the list

 @param list List handler
 @param item Element to remove from the list
 @param data Data stored in the deleted item

 @return -1 if the list handler is NULL or the list is empty otherwise 0
 */
int
dlist_remove(fes_dlist* list, fes_dlist_item* item, void** data);

/**
 @brief Move the item at the beginning of the list

 @param list List handler
 @param item Item to move

 @return -1 if the list handler is NULL or the list is empty otherwise 0
 */
int
dlist_move_front(fes_dlist* list, fes_dlist_item* item);

/**
 @brief Inserts a new element at the beginning of the list

 @param list List handler
 @param data Data to insert in the new element

 @return -1 if the list handler is NULL, empty or there is not enough memory
 to create a new item otherwise 0
 */
int
dlist_push_front(fes_dlist* list, const void* data);

/**
 @brief Removes the last element in the list

 @param list List handler
 @param data Data stored in the deleted item

 @return -1 if the list handler is NULL or the list is empty otherwise 0
 */
int
dlist_pop_back(fes_dlist* list, void** data);

/**
 @brief Get the number of items in the list

 @param list List handler

 @return the number of items in the list
 */
size_t
dlist_size(const fes_dlist* const list);

/**
 @brief Returns the first element in the list container.

 @param list List handler

 @return the first element
 */
fes_dlist_item*
dlist_head(const fes_dlist* const list);
