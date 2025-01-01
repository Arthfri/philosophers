#include "philo.h"

long    get_time(t_timecode timecode)
{
    struct timeval tv; // a voir

    if (gettimeofday(&tv, NULL))
        error_exit("Gettimeofday error");
    if (timecode == SECOND)
        return (tv.tv_sec + (tv.tv_usec / 1e6));
    else if (timecode == MILLISECOND)
        return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
    else if (timecode == MICROSECOND)
        return ((tv.tv_sec * 1e6) + tv.tv_usec);
    else
        error_exit("Wrong input to gettime");
    return (1377);
    
}

void    error_exit(const char *error)
{
    printf("%s \n", error);
    exit(EXIT_FAILURE);
}

void    ft_usleep(long usec, t_table *table)
{
    long    start;
    long    elapsed;
    long    rem;

    start = get_time(MICROSECOND);
    while (get_time(MICROSECOND) - start < usec)
        if (simulation_finished(table))
            break;
        elapsed = get_time(MICROSECOND) - start;
        rem = usec - elapsed;

        if ( rem > 1e3)
            usleep(rem / 2);
        else
        {
            while (get_time(MICROSECOND) - start < usec)
                ;
        }
            
}

void    clean(t_table *table)
{
    t_philo *philo;
    int i;

    i = 0;
    while (i++ < table->philo_nbr)
    {
        philo = table->philos + i;
        mutex_handle(&philo->philo_mtx, DESTROY);
    }
    mutex_handle(&table->table_mtx);
    mutex_handle(&table->write_mtx);
    free(table->forks);
    free(table->philos);
}