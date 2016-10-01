/*
 * heap.h
 * Copyright (C) 2016 DerShokus <lily.coder@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <stdbool.h>

typedef int (*heap_item_cmp)(const void *, const void *, const size_t);

struct heap {
        uint8_t *head; // head of a tree (head >= data) in bytes for simplify
                       //arithmetics
        uint8_t *data; // pointer to an array

        size_t size;      // count of stored items
        size_t item_size; // item's size
        size_t capasity;  // max size of an array

        heap_item_cmp cmp; // function to compare items
};

bool heap_init(struct heap *heap, void *data, const size_t capasity,
               const size_t item_size, heap_item_cmp cmp);

bool heap_push(struct heap *heap, void *item);

bool heap_pop(struct heap *heap, void *data, const size_t size);

#endif /* !HEAP_H */
