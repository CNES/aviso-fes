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

#include "cache.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <uthash.h>

#include "dlist.h"

fes_cache_item*
fes_new_cache_item(fes_grid* const grid, const size_t index) {
  fes_cache_item* item;

  // Allocate the new cache item
  if ((item = (fes_cache_item*) malloc(sizeof(fes_cache_item))) == NULL)
    return NULL;

  // Promote this new item in the queue
  dlist_push_front(&grid->buffer->list, item);

  // Update the properties of this item
  item->filled = 0;
  item->index = index;
  item->value = (fes_double_complex*) malloc(
      grid->n_grids * sizeof(fes_double_complex));
  item->list_item = dlist_head(&grid->buffer->list);

  // Saves the new entry in the associative array
  HASH_ADD_INT(grid->buffer->values, index, item);

  return item;
}

void fes_delete_cache_item(void* ptr) {
  fes_cache_item* item = (fes_cache_item*) ptr;

  free(item->value);
  free(item);
}

int fes_get_cache(fes_grid* const grid, const size_t i_lon, const size_t i_lat,
                  const size_t n, fes_double_complex* value) {
  fes_cache_item* item;

  size_t index =
      grid->transpose ?
          i_lon * grid->lat_dim + i_lat : i_lat * grid->lon_dim + i_lon;

  // Search this index in the cache
  HASH_FIND_INT(grid->buffer->values, &index, item);

  // If it exists and all values have been read
  if (item != NULL && item->filled) {
    // Promote this entry in the queue
    dlist_move_front(&grid->buffer->list, item->list_item);
    *value = item->value[n];
    return 1;
  }
  return 0;
}

int fes_set_cache(fes_grid* const grid, const size_t i_lon, const size_t i_lat,
                  const size_t n, const fes_double_complex* value) {
  fes_cache_item *item, *tail;

  size_t index =
      grid->transpose ?
          i_lon * grid->lat_dim + i_lat : i_lat * grid->lon_dim + i_lon;

  // Is it a new cache entry ?
  HASH_FIND_INT(grid->buffer->values, &index, item);
  if (item == NULL) {
    // If the buffer is full, the last item stored is deleted.
    if (dlist_size(&grid->buffer->list) > grid->buffer->max_size) {
      dlist_pop_back(&grid->buffer->list, (void**) &tail);
      HASH_DEL(grid->buffer->values, tail);
      fes_delete_cache_item(tail);
    }

    if ((item = fes_new_cache_item(grid, index)) == NULL)
      return 1;
  }

  // Stores the value read into the cache
  item->value[n] = *value;

  // Indicates whether the reading of grids is completed or not.
  item->filled = n == grid->n_grids - 1;
  return 0;
}
