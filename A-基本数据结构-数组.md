### A-基本数据结构


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