/*
 * heap.c
 * Copyright (C) 2016 DerShokus <lily.coder@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "heap.h"

#include <assert.h>
#include <strings.h>
#include <stdio.h>

bool heap_init(struct heap *heap, void *data, const size_t capasity,
               const size_t item_size, heap_item_cmp cmp)
{
        if (!heap || !data || !capasity || !item_size || !cmp)
                return false;

        heap->head = heap->data = data;
        heap->size              = 0;
        heap->item_size         = item_size;
        heap->capasity          = capasity;
        heap->cmp               = cmp;

        return true;
}

inline static void exchange(void *a, void *b, const size_t size)
{
        uint8_t c[size];
        memcpy(c, a, size); // uint8_t c = *a;
        memcpy(a, b, size); //*a        = *b;
        memcpy(b, c, size); //*b        = c;
}

inline static size_t node_head(size_t index)
{
        if (!index)
                return 0;
        return (index - 1) / 2;
}

static void heap_fix_up(struct heap *heap)
{
        uint8_t *tree = heap->head;
        for (size_t i = heap->size - 1; i > 0; i = node_head(i)) {
                const size_t h = node_head(i);
                if (heap->cmp(&tree[i * heap->item_size],
                              &tree[h * heap->item_size],
                              heap->item_size) < 0)
                        exchange(&tree[i], &tree[h], heap->item_size);
                else
                        break;
        }
}

bool heap_push(struct heap *heap, void *item)
{
        if (!heap || heap->size == heap->capasity) {
                printf("size: %lu == %lu\n", heap->size, heap->capasity);
                return false;
        }

        // printf("size: %lu, capcasity: %lu, item: %u\n",
        //        heap->size, heap->capasity, item);
        assert(heap->size < heap->capasity);
        const size_t real_size = ((heap->head - heap->data) + heap->size);
        assert(real_size <= heap->capasity);
        if (real_size == heap->capasity) {
                memmove(heap->data, heap->head, heap->size * heap->item_size);
                // for (size_t i = 0; i < heap->size; ++i) {
                //        heap->data[i] = heap->head[i];
                //}
                heap->head = heap->data;
        }
        /* set item as the last item of a heap and increment the size */
        memcpy(&heap->head[heap->size++], item, heap->item_size);
        ;
        /* finaly it needs a fixup an order */
        heap_fix_up(heap);

        return true;
}

//       [ 0                  [ 1
//    1          2            2   3
//  3   4     5     6        4 5 6 7 ]
// 7 8 9 10 11 12 13 14

static inline size_t node_left(size_t index) { return index * 2 + 1; }

static inline size_t node_right(size_t index) { return node_left(index) + 1; }

static void heap_fix_down(struct heap *heap)
{
        uint8_t *tree     = heap->head;
        const size_t size = heap->size;
        for (size_t i = 0; i < size; ++i) {
                const size_t left  = node_left(i);
                const size_t right = node_right(i);
                if (right < size && tree[left] > tree[right])
                        exchange(&tree[left], &tree[right], heap->item_size);
                if (left < size && tree[i] > tree[left])
                        exchange(&tree[i], &tree[left], heap->item_size);
        }
}

bool heap_pop(struct heap *heap, void *data, const size_t size)
{
        if (!heap || !heap->head || !heap->size || !data ||
            size != heap->item_size)
                return false;

        memcpy(data, heap->head, size);
        heap->head = heap->head + size;
        heap->size--;
        heap_fix_down(heap);

        return true;
}
