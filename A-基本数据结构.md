### ngx 基本数据结构


#### 整型

```c
typedef intptr_t        ngx_int_t;
typedef uintptr_t       ngx_uint_t;
typedef intptr_t        ngx_flag_t;

// 引用：ngx_config.h
```


----

#### 字符串类型


ngx字符串结构体： ngx_str_t

	len		整型， 字符个数，不包含NUL字符
	data 	指针类型， 字符指针(字符数组)


```c
typedef struct {
    size_t      len;
    u_char     *data;
} ngx_str_t;

// 引用：ngx_string.h
```

ngx 字符串初始化操作：
```c
#define ngx_string(str)     { sizeof(str) - 1, (u_char *) str }
#define ngx_null_string     { 0, NULL }
```

----

ngx键值类型结构体： ngx_keyval_t

	key 	ngx字符串类型，键
	value 	ngx字符串类型，值

```c
typedef struct {
    ngx_str_t   key;
    ngx_str_t   value;
} ngx_keyval_t;

// 引用：ngx_string.h
```



------

ngx变量值类型结构体： ngx_variable_value_t

	len				整型，字符串长度，最大2的28次方减一(268435455)
	valid			整型，是否有效
	no_cacheable	整型，是否不用缓存
	not_found		整型，查找不到变量
	data 			指针类型， 变量具体的值

```c
typedef struct {
    unsigned    len:28;

    unsigned    valid:1;
    unsigned    no_cacheable:1;
    unsigned    not_found:1;
    unsigned    escape:1;

    u_char     *data;
} ngx_variable_value_t;
```
----

ngx 字符串节点类型：ngx_str_node_t

	node 	ngx 红黑树节点类型，节点字段
	str 	ngx字符串类型，节点字符串

```c
typedef struct {
    ngx_rbtree_node_t         node;
    ngx_str_t                 str;
} ngx_str_node_t;
```