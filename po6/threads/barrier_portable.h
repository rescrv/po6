#include <pthread.h>
#include <inttypes.h>

#define pthread_barrier_t po6_barrier_t
#define pthread_barrier_attr_t po6_barrier_attr_t
#define pthread_barrier_init(b,a,n) po6_barrier_init(b,n)
#define pthread_barrier_destroy(b) po6_barrier_destroy(b)
#define pthread_barrier_wait(b) po6_barrier_wait(b)
#define PTHREAD_BARRIER_SERIAL_THREAD (-1)

typedef struct po6_barrier{
    pthread_mutex_t lock;
    pthread_cond_t cv;
    uint64_t height;
    uint64_t level; 
    uint64_t generation;
} po6_barrier_t;

int
po6_barrier_init(po6_barrier_t* bar, uint64_t height)
{
    bar->height = height;
    bar->level = 0;
    bar->generation = 0;
    if(pthread_mutex_init(&bar->lock, NULL) < 0)
    {
        return -1;
    }

    if(pthread_cond_init(&bar->cv, NULL) < 0)
    {
        return -1;
    }

    return 0;
}

int
po6_barrier_destroy(po6_barrier_t* bar)
{
    pthread_mutex_destroy(&bar->lock);
    pthread_cond_destroy(&bar->cv);
    return 0;
}

int
po6_barrier_wait(po6_barrier_t* bar)
{
    int cancelstate;
    uint64_t gen; 
    pthread_mutex_lock(&bar->lock);
    gen = bar->generation;
    ++bar->level;

    if(bar->level==bar->height)
    {
        bar->level = 0;
        ++bar->generation;
        pthread_cond_broadcast(&bar->cv);
        pthread_mutex_unlock(&bar->lock);
        return -1;
    }
    else
    {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cancelstate);

        while(bar->generation==gen)
        {
            pthread_cond_wait(&bar->cv, &bar->lock);
        }

        pthread_setcancelstate(cancelstate,NULL);
    }

}
