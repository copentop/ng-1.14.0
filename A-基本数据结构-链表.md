### A-基本数据结构


#### 链表

链表节点结构：ngx_list_part_s 

别名：ngx_list_part_t

	elts	指针类型，分配的地址起始地址
	nelts	整型类型，元素个数
	next	ngx链表节点类型，指向下个链表节点


```c
struct ngx_list_part_s {
    void             *elts;
    ngx_uint_t        nelts;
    ngx_list_part_t  *next;
};
```


链表结构： ngx_list_t

	last	ngx链表节点指针类型，指向最新可用的链表节点
	part	ngx链表节点类型，ngx链表节点头节点
	size	整型类型，元素占用的空间大小
	nalloc 	整型类型，链表单个节点容量
	pool 	ngx内存池指针类型， 指向当前分配空间的内存池


```c
typedef struct {
    ngx_list_part_t  *last;
    ngx_list_part_t   part;
    size_t            size;
    ngx_uint_t        nalloc;
    ngx_pool_t       *pool;
} ngx_list_t;
```


-----

#### 创建链表


创建逻辑：

1. 先创建ngx链表结构
2. 分配n个大小为size空间，初始化ngx链表的头节点。
3. ngx链表的last指向当前part域，part域持有分配元素的空间。

![img](https://github.com/copentop/ng-1.14.0/blob/master/struct_img/fun_list_create.jpg)



----

#### 链表扩容

当当前节点的元素个数等于ngx链表的容量(list.nalloc)时，
创建一个新的ngx链接节点，并根据容量分配元素的空间，同时修正
ngx链表的last指向新的节点。

```c
void *
ngx_list_push(ngx_list_t *l)
{
    void             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    // 元素个数和容量相等
    // 扩容
    if (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, sizeof(ngx_list_part_t));
        if (last == NULL) {
            return NULL;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        if (last->elts == NULL) {
            return NULL;
        }

        last->nelts = 0;
        last->next = NULL;

        l->last->next = last; 
        l->last = last;     // 修正list当前可用节点
    }

    elt = (char *) last->elts + l->size * last->nelts;
    last->nelts++;

    return elt;
}
```



![img](https://github.com/copentop/ng-1.14.0/blob/master/struct_img/fun_list_create.jpg)