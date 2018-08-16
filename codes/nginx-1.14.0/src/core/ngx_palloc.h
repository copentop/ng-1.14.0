
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)

/** 
 * 内存池清理函数指针
 */
typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

/**
 * ngx 内存池清理结构
 */
struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler; /* 清理的函数指针 */
    void                 *data; /* 数据指针 */
    ngx_pool_cleanup_t   *next; /* 清理结构 链表 */
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;

/** 
 * ngx 大内存池 结构
 */
struct ngx_pool_large_s {
    ngx_pool_large_t     *next; /* 执行ngx内存链表 */
    void                 *alloc; /* 分配的内存起始地址，free 用到 */
};

/**
 * ngx内存池数据结构
 */
typedef struct {
    u_char               *last;
    u_char               *end;
    ngx_pool_t           *next;     /* 下一个内存池 */
    ngx_uint_t            failed;
} ngx_pool_data_t;

/** 
 * ngx内存池结构
 */
struct ngx_pool_s {
    ngx_pool_data_t       d;    /* ngx数据域 */
    size_t                max;  /* 内存池可用的内存的最大值 */
    ngx_pool_t           *current;  /* 指向当前内存池(优先分配空间) */
    ngx_chain_t          *chain;    /* 指向一个 ngx_chain_t 结构 */
    ngx_pool_large_t     *large;    /* ngx大内存池 链表 */
    ngx_pool_cleanup_t   *cleanup; /* ngx清除域 */
    ngx_log_t            *log; /* ngx日志域 */
};


/**
 * ngx 文件清理 结构
 */
typedef struct {
    ngx_fd_t              fd; /* ngx 文件句柄域 */
    u_char               *name; /* 字符指针 */
    ngx_log_t            *log; /* ngx 日志域 */
} ngx_pool_cleanup_file_t;


ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
void ngx_destroy_pool(ngx_pool_t *pool);
void ngx_reset_pool(ngx_pool_t *pool);

void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
void ngx_pool_cleanup_file(void *data);
void ngx_pool_delete_file(void *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
