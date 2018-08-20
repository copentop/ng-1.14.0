#include <stdio.h>

#include "de_config.h"
#include "../nginx-1.14.0/src/core/ngx_array.h"

volatile ngx_cycle_t  *ngx_cycle;

void ngx_log_error_core(ngx_uint_t level, ngx_log_t *log, ngx_err_t err,
            const char *fmt, ...)
{
}

void dump_array(ngx_array_t* a)
{
    if (a)
    {
        printf("array = 0x%x\n", a);
        printf("  arr = %d\n", sizeof(ngx_array_t));
        printf("  .elts = 0x%x\n", a->elts);
        printf("  .nelts = %d\n", a->nelts);
        printf("  .size = %d\n", a->size);
        printf("  .nalloc = %d\n", a->nalloc);
        printf("  .pool = 0x%x\n", a->pool);

        printf("elements: ");
        int *ptr = (int*)(a->elts);
        for (; ptr < (int*)(a->elts + a->nalloc * a->size); )
        {
            printf("%d  ", *ptr++);
        }
        printf("\n");
    }
}

int main()
{
    ngx_pool_t *pool;
    ngx_pool_t *np;
    ngx_pool_large_t     *large;
    ngx_pool_data_t d;
    int i;
    int *ptr;
    int pool_size = 128;

    printf("--------------------------------\n");
    printf("create a new pool:\n");
    printf("--------------------------------\n");
    pool = ngx_create_pool(pool_size, NULL);

    printf("  pool_size = %d\n", pool_size);
    printf("  .pool = %d\n", sizeof(ngx_pool_t));
    printf("  .size = %d\n", pool->max);
    printf("  .pool = %p\n", pool);
    printf("  .pool.d = %d\n", sizeof(ngx_pool_data_t));
    printf("  pool.d.last = %p\n", pool->d.last);
    printf("  void * = %d\n", sizeof(void *));
    printf("  pool.large = %p\n", pool->large);

    printf("--------------------------------\n");
    printf("alloc an array from the pool:\n");
    printf("--------------------------------\n");
    ngx_array_t *a = ngx_array_create(pool, 5, sizeof(int));

    printf(" new a: pool.d.last = %p\n", pool->d.last);

    for (i = 0; i < 5; i++)
    {
        ptr = ngx_array_push(a);
        *ptr = 2*i;
    }



    dump_array(a);

    for(i=0; i < 8; i++) {
        ptr = ngx_array_push(a);
        if(ptr != NULL) {
            *ptr = 5*i;
        } else {
            printf(" a null \n");
        }
    }

     printf("--------------------------------\n");

    printf("  pool.d.last = %p\n", pool->d.last);
    printf("  pool.d.end = %p\n", pool->d.end);

    dump_array(a);

    printf("--------------------------------\n");
    printf("  pool.d.last = %p\n", pool->d.last);
    printf("  pool.d.end = %p\n", pool->d.end);

    printf("--------------------------------\n");
    np = pool;
    for(large = np->large; large; large = large->next) {
        printf("  pool.large = %p\n", large->alloc);
    }

    printf("--------------------------------\n");
    np = pool;
    for(; np->d.next; np = np->d.next) {
        printf("  n d.last = %p\n", np->d.last);
        printf("  n d.end = %p\n", np->d.end);
    }

    ngx_array_destroy(a);
    ngx_destroy_pool(pool);
    return 0;
}



