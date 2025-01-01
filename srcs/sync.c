#include "philo.h"

void    wait_threads(t_table *table)
{
    while (!get_bool(&table->table_mtx, &table->threads_rdy))
        ;
}

bool    all_threads_running(t_mtx *mtx, long *threads, long philo_nbr)
{
    bool    res;

    res = false;
    mutex_handle(mtx, LOCK);
    if (*threads == philo_nbr)
        res = true;
    mutex_handle(mtx, UNLOCK);
    return (res);
}

void    inc_long(t_mtx *mtx, long *value)
{
    mutex_handle(mtx, LOCK);
    (*value)++;
    mutex_handle(mtx, UNLOCK);
}

void    desync_philos(t_philo *philo)
{
    if (philo->table->philo_nbr % 2 == 0)
    {
        if (philo->id % 2 == 0)
            ft_usleep(3e4, philo->table);
    }
    else
    {
        if (philo->id % 2)
            thinking(philo, true);
    }
}