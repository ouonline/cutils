#include <stdio.h>
#include <pthread.h>
#include "atomic.h"

#ifdef USE_ASM

static atomic64_t counter;

#else

static long counter;

#ifdef USE_MUTEX
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#elif defined USE_SPINLOCK
static pthread_spinlock_t spinlock;
#endif

#endif

void* func(void* nil)
{
    int i;

    for (i = 0; i < 100000; ++i) {
#ifdef USE_ASM
        atomic64_inc(&counter);
#elif defined USE_MUTEX
        pthread_mutex_lock(&mutex);
        ++counter;
        pthread_mutex_unlock(&mutex);
#elif defined USE_SPINLOCK
        pthread_spin_lock(&spinlock);
        ++counter;
        pthread_spin_unlock(&spinlock);
#elif defined USE_BUILTIN
        __sync_fetch_and_add(&counter, 1);
#else
        ++counter;
#endif
    }

    return NULL;
}

#define NR_THREAD 10000

int main(void)
{
    int i;
    pthread_t pid[NR_THREAD];

#ifdef USE_SPINLOCK
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_SHARED);
#endif

    for (i = 0; i < NR_THREAD; ++i)
        pthread_create(&pid[i], NULL, func, NULL);

    for (i = 0; i < NR_THREAD; ++i)
        pthread_join(pid[i], NULL);

#ifdef USE_ASM
    printf("result: %ld\n", atomic64_read(&counter));
#else
    printf("result: %ld\n", counter);
#endif

    return 0;
}
