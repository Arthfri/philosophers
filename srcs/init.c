#include "philo.h"

static void assign_forks(t_philo *philo, t_fork *forks, int position)
{
    int philo_nbr;

    philo_nbr = philo->table->philo_nbr;

    philo->first_fork = &forks[(position + 1) % philo_nbr];
    philo->second_fork = &forks[position];
    if(philo->id % 2 == 0)
    {
        philo->first_fork = &forks[position];
        philo->second_fork = &forks[(position + 1) % philo_nbr];
    }
    
}

static void philo_init(t_table *table)
{
    int i;
    t_philo *philo;

    i = 0;
    while (i++ < table->philo_nbr)
    {
        philo = table->philos + i;
        philo->id = i + 1;
        philo->meals_counter = 0;
        philo->full = false;
        philo->table = table;
        mutex_handle(&philo->philo_mtx, INIT);
        assign_forks(philo, table->forks, i);

    }
}

void    data_init(t_table *table)
{
    int i;

    i = 0;
    table->end_simulation = false;
    table->threads_rdy = false;
    table->threads_run_nbr = 0;
    table->philos = safe_malloc(sizeof(t_philo) * table->philo_nbr);
    table->forks = safe_malloc(sizeof(t_fork) * table->philo_nbr);
    mutex_handle(&table->table_mtx, INIT);
    mutex_handle(&table->write_mtx, INIT);
    while (i++ < table->philo_nbr)
    {
       mutex_handle(&table->forks[i].fork_mtx, INIT);
        table->forks[i].fork_id = i; // debug
    }
    philo_init(table);
}