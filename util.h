#pragma once

typedef struct
{
    int size_used;
    int size_alloc;
    struct *array;

    void init_array(struct type)
    {
        array = malloc(sizeof(type));
        if(array == NULL)
            return 1;
        else
            size_alloc = 1;
    }

    void add(Vector *v, struct toadd)
    {
        if(size_used < size_alloc)
            v->array[v->size_used] = toadd;
        else
        {
            v->size *= 2;
            v->array = (struct*)realloc(v->array, v->size_alloc * sizeof(toadd));
            v->array[v->size_used] = toadd;
        }
        size_used++;
    }
}Vector;
