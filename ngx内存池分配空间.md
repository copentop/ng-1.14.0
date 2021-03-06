### ngx 内存池分配空间


### 小块内存分配逻辑

小内存块分配：

1. 遍历内存池链表，直到找到内存池d数据域的 last 到end 有符合大小的空间，则分配。
2. 如果整个内存池链表的d数据域都不能分配，则创建新内存池，在新内存池中分配大小，并将第一块内存池的current执行新的内存池。


新内存池分配：

分配新内存池逻辑：
1. 根据第一块内存池大小，分配一块新内存池，转换为 ngx_pool_s 类型。
2. 初始化新内存池的d数据域，d.last 执行d数据域的结束位置，作为此块内存池可用内存的起始地址，d.end 执行，执行整块内存池的结尾。
3. 在d.last 到 d.end 直接分配size 大小的内存。
4. 第一块内存池的current执行当前新分配的内存池，代码当前可分配空间的内存池。


备注：

除了内存池链表的第一块内存池的d数据域后面的字段有用之前，其他的内存池的d字段后面的字段都不再使用，
全部作为数据域使用。


![img](https://github.com/copentop/ng-1.14.0/blob/master/struct_img/fun_pool_palloc.jpg)



### 内存池创建大内存块


创建条件：如果申请的内存size 大于pool->max，则创建大内存块。

创建逻辑：

1. 先向系统申请size大小的内存。
2. 在内存池pool可用的内存中申请ngx_pool_large_s的结构体空间，初始化该结构体
的alloc指向新分配的内存。
3. 如果大内存块链表next指针为null 或块数大于5时，修正pool->large指向最新分配的
ngx大内存结构的起始地址。


![img](https://github.com/copentop/ng-1.14.0/blob/master/struct_img/fun_pool_palloc_large.jpg)





