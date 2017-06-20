#ifndef __SEARCH_CTX_H
#define __SEARCH_CTX_H

#include <sched.h>

#include "redismodule.h"
#include "spec.h"
#include "trie/trie_type.h"
#include <time.h>

#define CONCURRENT_SEARCH_POOL_SIZE 200

typedef struct {
  long long ticker;
  struct timespec lastTime;
  RedisModuleCtx *ctx;
} ConcurrentSearchCtx;

#define CONCURRENT_TICKS_PER_SWITCH 10

void ConcurrentSearch_CheckTimer(ConcurrentSearchCtx *ctx);

#define CONCURRENT_CTX_TICK(x)                                 \
  {                                                            \
    if (x && ++x->ticker % CONCURRENT_TICKS_PER_SWITCH == 0) { \
      ConcurrentSearch_CheckTimer(x);                          \
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
