#include "philo.h"

static bool philo_died(t_philo *philo)
{
    long    elapsed;
    long    to_die;

    if (get_bool(&philo->philo_mtx, &philo->full))
        return (false);
    elsapsed = get_time(MILLISECOND) - get_long(&philo->philo_mtx, &philo->last_meal_time);
    to_die = philo->table->time_to_die / 1e3;
    if (elapsed > to_die)
        return (true);
    return (false);
}
void    *monitor_dinner(void *data)
{
    int i;
    t_table *table;

    table = (t_table *)data;

    while (!all_threads_running(&table->table_mtx, &table->threads_run_nbr, table->philo_nbr))
        ;
    while (!simulation_finished(table))
    {
        i = 0;
        while (i++ < table->philo_nbr && !simulation_finished(table))
        {
            if (philo_died(table->philos + i))
            {
                set_bool(&table->table_mtx, &table->end_simulation, true);
                write_status(DIED, table->philos + i, DEBUG);
            }
        }
    }
    return (NULL);
}