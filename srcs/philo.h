#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h> // mutex/ init destroy lock unlock
                    // thrad : create, join, detach
#include <sys/time.h> // gettimeofday
#include <limits.h>

#define DEBUG   0

// ENUM
typedef enum e_opcode
{
    LOCK,
    UNLOCK,
    INIT,
    DESTROY,
    CREATE,
    JOIN,
    DETACH,
}       t_opcode;

typedef enum e_timecode
{
    SECOND,
    MILLISECOND,
    MICROSECOND,
}       t_timecode;

typedef enum e_status
{
    EATING,
    SLEEPING,
    THINKING,
    TAKE_FIRST_FORK,
    TAKE_SECOND_FORK,
    DIED,
}       t_philo_status;

typedef pthread_mutex_t t_mtx;

typedef struct  s_table t_table;

typedef struct s_fork
{
    t_mtx   fork_mtx;
    int fork_id; // debug
}       t_fork;

typedef struct s_philo
{
    int id;
    long    meals_counter;
    bool    full;
    long    last_meal_time;
    t_fork  *first_fork;
    t_fork  *second_fork;
    pthread_t   thread_id;
    t_mtx   philo_mtx;
    t_table *table;
}       t_philo;

struct s_table
{
    long    philo_nbr;
    long    time_to_die;
    long    time_to_eat;
    long    time_to_sleep;
    long    nbr_limit_meals;
    long    start_simulation;
    long    threads_run_nbr;
    bool    end_simulation;
    bool    threads_rdy;
    pthread_t   monitor;
    t_mtx   table_mtx;
    t_mtx   write_mtx;
    t_fork  *forks;
    t_philo *philos;
};


void    error_exit(const char *error);

void    parse_input(t_table *table, char **av);

void    *safe_malloc(size_t bytes);
void    mutex_handle(t_mtx *mutex, t_opcode opcode);
void    thread_handle(pthread_t *thread, void *(*foo)(void *), void *data,t_opcode opcode);

void    data_init(t_table *table);

void    set_bool(t_mtx *mtx, bool *dest, bool value);
bool    get_bool(t_mtx *mtx, bool *value);
void    set_long(t_mtx *mtx, long *dest, long value);
long    get_long(t_mtx *mtx, long *value);
bool    simulation_finished(t_table *table);

void    wait_threads(t_table *table);

long    get_time(t_timecode timecode);

void    ft_usleep(long usec, t_table *table);

void    write_status(t_philo_status status, t_philo *philo, bool debug);

void    dinner_start(t_table *table);
void    dinner_simulation(void *data);
void    *alone(void *arg);
void    thinking(t_philo *philo, bool before_sim);
bool    all_threads_running(t_mtx *mtx, long *threads, long philo_nbr);
void    inc_long(t_mtx *mtx, long *value);

void    clean(t_table *table);