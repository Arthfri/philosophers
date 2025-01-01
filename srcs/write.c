#include "philo.h"

static void    ws_debug(t_philo_status status, t_philo *philo, long elapsed)
{
    if (status == TAKE_FIRST_FORK && !simulation_finished(philo->table))
            printf("%-6ld %d has taken the 1 fork\t\t\tn° %d\n", elapsed,
                philo->id, philo->first_fork->fork_id);
    else if (status == TAKE_SECOND_FORK && !simulation_finished(philo->table))
            printf("%-6ld %d has taken the 2 fork\t\t\tn° %d\n", elapsed,
                philo->id, philo->second_fork->fork_id);
    else if (status == EATING && !simulation_finished(philo->table))
            printf("%-6ld %d is eating\t\t\t %ld\n", elapsed, philo->id, philo->meals_counter);
    else if (status == THINKING && !simulation_finished(philo->table))
            printf("%-6ld %d is thinking\n", elapsed, philo->id);
    else if (status == SLEEPING && !simulation_finished(philo->table))
            printf("%-6ld %d is sleaping\n", elapsed, philo->id);
    else if (status == DIED)
            printf("%-6ld %d is died\n", elapsed, philo->id);
}

void    write_status(t_philo_status status, t_philo *philo, bool debug)
{
    long    elapsed;

    elapsed = get_time(MILLISECOND) - philo->table->start_simulation;
    if (philo->full)
        return ;
    mutex_handle(&philo->table->write_mtx, LOCK);
    if (debug)
        ws_debug(status, philo, elapsed);
    else
    {
        if ((status == TAKE_FIRST_FORKstatus == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
                && !simulation_finished(philo->table))
            printf("%-6ld %d has taken a fork\n", elapsed, philo->id);
        else if (status == EATING && !simulation_finished(philo->table))
            printf("%-6ld %d is eating\n", elapsed, philo->id);
        else if (status == THINKING && !simulation_finished(philo->table))
            printf("%-6ld %d is thinking\n", elapsed, philo->id);
        else if (status == SLEEPING && !simulation_finished(philo->table))
            printf("%-6ld %d is sleaping\n", elapsed, philo->id);
        else if (status == DIED)
            printf("%-6ld %d is died\n", elapsed, philo->id);
    }
    mutex_handle(&philo->table->write_mtx, UNLOCK);

}