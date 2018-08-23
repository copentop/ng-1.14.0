### A-基本数据结构

数组
    ngx_array.h
    ngx_array.c


#### 数组

ngx数组结构体：ngx_array_t

	elts	指针类型，数组元素的首地址
	nelts	整型类型，实际数组元素个数
	size	整型类型，每个元素占用内存的大小
	nalloc	整型类型，数组容量大小，可按倍数扩展
	pool	ngx内存池类型，分配数组空间的内存池，数组不直接从系统申请。

```c
typedef struct {
    void        *elts;
    ngx_uint_t   nelts;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;
```

-----

#### 数组操作

+ 数组初始化

数组a 从ngx内存池pool中申请 n个空间，每个空间大小为size。

```c
static ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * size);
    if (array->elts == NULL) {
        return NGX_ERROR;
    }

    return NGX_OK;
}
```

----

+ 创建数组

从ngx内存池中创建ngx数组结构体本身的空间,
然后再创建 n个，每个大小为size的数组空间

```c
ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size)
{
    ngx_array_t *a;

    a = ngx_palloc(p, sizeof(ngx_array_t));
    if (a == NULL) {
        return NULL;
    }

    if (ngx_array_init(a, p, n, size) != NGX_OK) {
        return NULL;
    }

    return a;
}
```

![img](https://github.com/copentop/ng-1.14.0/blob/master/struct_img/fun_array_create.jpg)


----

+ 数组添加元素

在数组a中添加元素时，返回该元素的起始地址，由调用者
填充数据。

当数组需要扩容时，有两种情况发生：
1. 如果内存池还有可用空间，而且可用空间是数组的末尾地址连续空间，则在内存可用空间上
分配数组元素的地址。
2. 如果地址是不连续的时，则申请原来数组大小2倍空间，有可能在小块内存上申请或大块内存上
申请，并复制旧数据，同时修正数组elts的指向。


```c
void *
ngx_array_push(ngx_array_t *a)
{
    void        *elt, *new;
    size_t       size;
    ngx_pool_t  *p;

    // 当前数组元素个数等于数组的容量，需要扩容
    if (a->nelts == a->nalloc) {

        /* the array is full */
        // 数组空间大小
        size = a->size * a->nalloc;
        // 内存池
        p = a->pool;

        // 内存池上可用地址的起始地址是数组的元素末尾地址(避免数组地址不连续)，
        // 而且可用地址允许分配数组元素
        if ((u_char *) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } else {
            /* allocate a new array */

            new = ngx_palloc(p, 2 * size); // 扩容为原来的一倍
            if (new == NULL) {
                return NULL;
            }

            ngx_memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (u_char *) a->elts + a->size * a->nelts;
    a->nelts++;

    return elt;
}
```

----

#### 数组扩容例子


ngx数组扩容时，可能在小块内存上分配，也有可能在大块内存上分配。

如果扩展需要在大块内存上分配时，会操作内存池浪费空间(旧的小块内存上的数组元素不再使用)。

结果：
![img](https://github.com/copentop/ng-1.14.0/blob/master/struct_img/ngx_array_test_01.jpg)

代码：

    codes/examples/array_test_01.c

```
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
    int pool_size = 128;    // 内存池总大小

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

    for(i=0; i < 8; i++) {      // 扩容
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
```