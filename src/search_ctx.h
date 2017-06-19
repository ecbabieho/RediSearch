#ifndef __SEARCH_CTX_H
#define __SEARCH_CTX_H

#include <sched.h>

#include "redismodule.h"
#include "spec.h"
#include "trie/trie_type.h"

typedef struct {
  long long ticker;
  RedisModuleCtx *ctx;
} ConcurrentSearchCtx;

#define CONCURRENT_TICKS_PER_SWITCH 10000

#define CONCURRENT_CTX_TICK(x)                                 \
  {                                                            \
    if (x && ++x->ticker % CONCURRENT_TICKS_PER_SWITCH == 0) { \
      RedisModule_ThreadSafeContextUnlock(x->ctx);             \
      sched_yield();                                           \
      RedisModule_ThreadSafeContextLock(x->ctx);               \
    }                                                          \
  }

/** Context passed to all redis related search handling functions. */
typedef struct {
  RedisModuleCtx *redisCtx;
  IndexSpec *spec;
} RedisSearchCtx;

RedisSearchCtx *NewSearchCtx(RedisModuleCtx *ctx, RedisModuleString *indexName);
void SearchCtx_Free(RedisSearchCtx *sctx);
#endif
