#ifndef ASYNC_H_
#define ASYNC_H_

#include <stdint.h>
#include <stdbool.h>

#define ASYNC_TASK_TABLE_SZ 256
#define CONCAT2(A,B) A ## B
#define CONCAT(A,B) CONCAT2(A,B)

typedef int32_t async_task;

typedef struct _async_state {
    int32_t curr_state;
    int32_t final_state;
    async_task waiting_task;
    struct _async_state (*fn)(struct _async_state s, void *);
    void *payload;
} async;

async ASYNC_TASK_TABLE[ASYNC_TASK_TABLE_SZ] = {0};
uint32_t ASYNC_TASK_TABLE_I = 0;

#define ASYNC_CLEAN()\
    memset(ASYNC_TASK_TABLE, 0, sizeof(ASYNC_TASK_TABLE[0])*ASYNC_TASK_TABLE_SZ); \
    ASYNC_TASK_TABLE_I = 0;
#define ASYNC_RUN()\
    bool finished = 0;\
    while(!finished){ \
        finished = 1;\
        for(int i = 0; i < ASYNC_TASK_TABLE_SZ; i++){ \
            if(ASYNC_TASK_TABLE[i].curr_state != ASYNC_TASK_TABLE[i].final_state && ASYNC_TASK_TABLE[i].fn){\
                ASYNC_TASK_TABLE[i] = ASYNC_TASK_TABLE[i].fn(ASYNC_TASK_TABLE[i], ASYNC_TASK_TABLE[i].payload); \
                finished = 0; \
            } \
        } \
    } \
    ASYNC_CLEAN();
#define ASYNC_CALL(f, p)({ \
    if(ASYNC_TASK_TABLE_I+1 < ASYNC_TASK_TABLE_SZ){ \
        ASYNC_TASK_TABLE_I++;\
        ASYNC_TASK_TABLE[ASYNC_TASK_TABLE_I] = (async){ \
            .curr_state = 0, \
            .final_state = -1, \
            .fn = (async (*)(async, void*)) f, \
            .payload = p, \
            .waiting_task = -1 \
        }; \
    }else{\
        KERROR("ASYNC_TASK_TABLE overflow\n");\
    }\
    ASYNC_TASK_TABLE_I; \
})
#define ASYNC_YIELD() return state
#define ASYNC_OVER()\
    state.waiting_task = -1;\
    state.curr_state = -1;\
    return state;
#define ASYNC_BODY(statements) \
    switch(state.curr_state){ \
        case 0: \
        statements \
    }\
    ASYNC_OVER();
#define ASYNC_AWAIT_L(t,l) \
    state.waiting_task = t; \
    state.curr_state = l; \
    ASYNC_YIELD(); \
    case l:{ \
        if(state.waiting_task != -1){ \
            if(ASYNC_TASK_TABLE[state.waiting_task].curr_state != ASYNC_TASK_TABLE[state.waiting_task].final_state){ \
                state.curr_state = l; \
                ASYNC_YIELD(); \
            } \
        }\
    }

#define ASYNC_AWAIT(t) ASYNC_AWAIT_L(t,__LINE__)
#define ASYNC_AWAIT_THEN(t, expr) \
    ASYNC_AWAIT(t) \
    expr
#define ASYNC_FN_H(name, payload) async name(async state, payload)
#define ASYNC_FN(name, payload, body) ASYNC_FN_H(name, payload){ ASYNC_BODY( body ) }
#endif
