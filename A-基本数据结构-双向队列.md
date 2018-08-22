### A-基本数据结构


#### 双向队列

ngx双向队列构体： ngx_queue_s

	prev	ngx队列指针类型，前置节点
	next	ngx队列指针类型，后缀节点

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

在节点后面插入新节点：

对于节点来说： 节点的后面节点为新节点，新节点为节点后面节点的前置节点

1. 拆掉节点next之前，保存指向，即新节点的next为节点next：
x->next = h->next

2. 新节点作为节点后面的节点的前置节点:
x->next-prev = x;


对于节点来说：新节点的前置节点为该节点，该节点的后面节点为新节点

1. 新节点的前置节点为该节点
x->prev = h

2. 该节点的后面节点为新节点
h->next = x