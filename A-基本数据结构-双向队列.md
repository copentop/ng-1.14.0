### A-基本数据结构


双向队列

    ngx_queue.h
    ngx_queue.c

#### 双向队列

ngx双向队列构体： ngx_queue_s

	prev	ngx队列指针类型，前置节点
	next	ngx队列指针类型，后置节点

```c
typedef struct ngx_queue_s  ngx_queue_t;

struct ngx_queue_s {
    ngx_queue_t  *prev;
    ngx_queue_t  *next;
};
```


ngx双向队列没有数据域，只保存队列的属性，数据域有具体业务实现。

-----

#### 节点后面插入节点

```c
#define ngx_queue_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x
```

在节点后面插入新节点：

对于节点来说： 节点的后面节点为新节点，新节点为节点后面节点的前置节点

1. 拆掉节点next之前，保存指向，即新节点的next为节点next：
x->next = h->next

2. 新节点作为节点后面的节点的前置节点:
x->next-prev = x;


对于新节点来说：新节点的前置节点为该节点，该节点的后面节点为新节点

1. 新节点的前置节点为该节点
x->prev = h

2. 该节点的后面节点为新节点
h->next = x


----

#### 节点前面插入节点


```c
#define ngx_queue_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x
```

在节点前面插入新节点

对于节点来说，节点的前节点为新节点，新节点为节点前面节点的后置节点

1.拆掉节点的pre之前，需要保存指向，即新节点的prev为节点的prev：
x->prev = h->prev

2. 新节点为节点前面节点的后置节点：
x->prev-next = x;

对于新节点来说：新节点的后置节点为该节点，该节点的前节点为新节点:

1. 新节点的后置节点为该节点：
x->next = h;

2. 该节点的前面节点为新节点：
h->prev = x;

-----

#### 删除节点

```c
#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next
```

一个节点有prev和next, 删除节点即拆掉prev和next即可，而且保持队列的指向不变


1. 去掉prev时，需要该节点的后置节点prev指向该节点的前置节点:
x->next->prev = x->prev;

2. 去掉next时，需要该节点的前置节点next指向该节点的后置节点：
x->prev->next = x->next;



