#include "philo.h"

void    set_bool(t_mtx *mtx, bool *dest, bool value)
{
    mutex_handle(mtx, LOCK);
    *dest =  value;
    mutex_handle(mtx, UNLOCK);
}

bool    get_bool(t_mtx *mtx, bool *value)
{
    bool    res;

    mutex_handle(mtx, LOCK);
    res = *value;
    mutex_handle(mtx, UNLOCK);
    return (res);
}

void    set_long(t_mtx *mtx, long *dest, long value)
{
    mutex_handle(mtx, LOCK);
    *dest =  value;
    mutex_handle(mtx, UNLOCK);
}

long    get_long(t_mtx *mtx, long *value)
{
    long    res;

    mutex_handle(mtx, LOCK);
    res = *value;
    mutex_handle(mtx, UNLOCK);
    return (res);
}

bool    simulation_finished(t_table *table)
{
    return (get_bool(&table->table_mtx, &table->end_simulation));
}