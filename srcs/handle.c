#include "philo.h"

void    *safe_malloc(size_t bytes)
{
    void    *res;

    res = malloc(bytes);
    if ( res == NULL)
        error_exit("Error malloc");
    return (res);
}

static void mutex_error(int status, t_opcode opcode)
{
    if (status == 0) // mutex is valid
        return ;
    if (status == EINVAL && (opcode == LOCK || opcode == UNLOCK))
        error_exit("Value specified by mutex is invalid / LOCK & UNLOCK");
    else if (status == EINVAL && opcode == INIT)
        error_exit("Value specified by attr is invalid / INIT");
    else if (status == EDEADLK)
        error_exit("A deadlock would occur if the thread blocked waiting for mutex");
    else if (status == EPERM)
        error_exit("The current thread does not hold a lock on mutex");
    else if (status == ENOMEM)
        error_exit("The process cannot allocat enough memory to create another mutex");
    else if (status == EBUSY)
        error_exit("Mutex is locked");
}

void    mutex_handle(t_mtx *mutex, t_opcode opcode)
{
    if (opcode == LOCK)
        mutex_error(pthread_mutex_lock(mutex), opcode); // a voir
    else if (opcode == UNLOCK)
        mutex_error(pthread_mutex_unlock(mutex), opcode);
    else if (opcode == INIT)
        mutex_error(pthread_mutex_init(mutex), opcode);
    else if (opcode == DESTROY)
        mutex_error(pthread_mutex_destroy(mutex), opcode);
    else
        error_exit("Wrong opcode for mutex handle");
}

static void thread_error(int status, t_opcode opcode)
{
    if (status == 0)
        return ;
    if (status == status == EAGAIN)
        error_exit("No ressources to create another thread");
    else if (status == EPERM)
        error_exit("the caller have not appropriate permission");
    else if (status == EINVAL && (JOIN == opcode || DETACH == opcode))
        error_exit("The value specified by thread is not joinable");
    else if (status == EINVAL && CREATE == opcode)
        error_exit("The value specified by attr is invalid");
    else if (status == ESRCH)
        error_exit("No thread could be found corresponding to that"
                "specified by the given thread ID");
    else if (status == EDEADLK)
        error_exit("A deadlock was detected or the value of thread specifies the calling thread");
} 

void   thread_handle(pthread_t *thread, void *(*foo)(void *), void *data, t_opcode opcode)
{
    if (opcode == CREATE)
        thread_error(pthread_create(thread, NULL, foo, data), opcode);
    else if (opcode == JOIN)
        thread_error(pthread_join(*thread, NULL), opcode);
    else if (opcode == DETACH)
        thread_error(pthread_detach(*thread), opcode);
    else
        error_exit("wrong opcode for thread handle");
}