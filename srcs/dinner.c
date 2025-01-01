#include "philo.h"

static void eat(t_philo *philo)
{
    mutex_handle(&philo->first_fork->fork_mtx, LOCK);
    write_status(TAKE_FIRST_FORK, philo, DEBUG);
    mutex_handle(&philo->second_fork->fork_mtx, LOCK);
    write_status(TAKE_SECOND_FORK, philo, DEBUG);
    set_long(&philo->philo_mtx, &philo->last_meal_time, get_time(MILLISECOND));
    philo->meals_counter++;
    write_status(EATING, philo, DEBUG);
    ft_usleep(philo->table->time_to_eat, philo->table);
    if (philo->table->nbr_limit_meals > 0 && philo->meals_counter == philo->table->nbr_limit_meals)
        set_bool(&philo->philo_mtx, &philo->full, true);
    mutex_handle(&philo->first_fork->fork_mtx, UNLOCK);
    mutex_handle(&philo->second_fork->fork_mtx, UNLOCK);

}

void    thinking(t_philo *philo, bool before_sim)
{
    long    t_to_eat;
    long    t_to_sleep;
    long    t_to_think;

    if (!before_sim)
        write_status(THINKING, philo, DEBUG);
    if (philo->table->philo_nbr % 2 == 0)
        return ;
    t_to_eat = philo->table->time_to_eat;
    t_to_sleep = philo->table->time_to_sleep;
    t_to_think = t_to_eat * 2 - t_to_sleep;
    if (t_to_think < 0)
        t_to_think = 0;
    ft_usleep(t_to_think * 0.42, philo->table);
}

void    *alone(void *arg)
{
    t_philo *philo;

    philo = (* philo)arg;
    wait_threads(philo->table);
    set_long(&philo->philo_mtx, &philo->last_meal_time, get_time(MILLISECOND));
    inc_long(&philo->table->table_mtx, &philo->table->threads_run_nbr);
    write_status(TAKE_FIRST_FORK, philo, DEBUG);
    while (!simulation_finished(philo->table))
        usleep(200);
    return (NULL);
}

void    *dinner_simulation(void *data)
{
    t_philo *philo;

    philo = (t_philo *)data;

    wait_threads(philo->table);
    set_long(&philo->philo_mtx, &philo->last_meal_time, get_time(MILLISECOND));
    inc_long(&philo->table->table_mtx, &philo->table->threads_run_nbr);
    desync_philos(philo);
    while (!simulation_finished(philo->table))
    {
        if (philo->full)
            break;
        eat(philo);
        write_status(SLEEPING, philo, DEBUG);
        ft_usleep(philo->table->time_to_sleep, philo->table);
        thinking(philo, false);
    }

    return (NULLs);
}

void    dinner_start(t_table *table)
{
    int i;

    i = 0;
    if (table->nbr_limit_meals == 0)
        return ; // back to clean
    else if (table->philo_nbr == 1)
        thread_handle(table->philos[0].thread_id, alone, &table->philos[0], CREATE);
    else
    {
        while (i++ < table->philo_nbr)
            thread_handle(&table->philos[i].thread_id, dinner_simulation, &table->philos[i], CREATE);
    }
    thread_handle(&table->monitor, monitor_dinner, table, CREATE);
    table->start_simulation = get_time(MILLISECOND);
    set_bool(&table->table_mtx, &table->threads_rdy, true);

    i = 0;
    while (i++ < table->philo_nbr)
        thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN);
    set_bool(&table->table_mtx, &table->end_simulation, true);
    thread_handle(&table->monitor, NULL, NULL, JOIN);
    
}