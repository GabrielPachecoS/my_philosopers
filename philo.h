/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:51:04 by gapachec          #+#    #+#             */
/*   Updated: 2025/06/08 21:14:03 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

/**
 * @brief Represents a single philosopher in the simulation.
 * * Each philosopher has an ID, tracks meals eaten, their last meal time,
 * the IDs of their left and right forks, their thread ID, and a pointer
 * back to the global rules.
 * * @param id The unique identifier of the philosopher.
 * @param meals_eaten The number of meals the philosopher has consumed.
 * @param last_meal_time The timestamp (in milliseconds) of the philosopher's last meal.
 * @param left_fork_id The index of the left fork in the forks array.
 * @param right_fork_id The index of the right fork in the forks array.
 * @param thread_id The pthread ID for this philosopher's thread.
 * @param rules A pointer to the global simulation rules.
 */
typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	int				left_fork_id;
	int				right_fork_id;
	pthread_t		thread_id;
	struct s_rules	*rules;
}	t_philo;

/**
 * @brief Defines the global rules and state for the simulation.
 * * This structure holds all parameters and shared resources for the simulation,
 * including philosopher counts, timing, mutexes for synchronization,
 * and flags for simulation termination.
 * * @param num_philo The total number of philosophers.
 * @param time_to_die The time (in milliseconds) a philosopher can go without eating before dying.
 * @param time_to_eat The time (in milliseconds) it takes a philosopher to eat.
 * @param time_to_sleep The time (in milliseconds) it takes a philosopher to sleep.
 * @param must_eat_count The number of times each philosopher must eat (or -1 if not specified).
 * @param time_initial The starting timestamp of the simulation.
 * @param all_ate A flag to indicate if all philosophers have eaten enough. (Note: `someone_died` is often used to terminate instead)
 * @param someone_died A flag to indicate if any philosopher has died, used to stop the simulation.
 * @param philos A dynamically allocated array of philosopher structures.
 * @param forks A dynamically allocated array of mutexes, representing the forks.
 * @param print_mutex A mutex to protect stdout during print operations.
 * @param state_mutex A mutex to protect shared simulation state (e.g., `last_meal_time`, `someone_died`).
 * @param monitor_thread The pthread ID for the monitoring thread.
 */
typedef struct s_rules
{
	int				num_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;
	long			time_initial;
	int				all_ate;
	int				someone_died;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
	pthread_t		monitor_thread;
}	t_rules;

int		is_valid_number(const char *str);
int		check_args_validity(int argc, char **argv);
int		init_rules(t_rules *rules, int argc, char **argv);
void	*monitor_routine(void *arg);
void	*philo_routine(void *arg);
void	safe_usleep(long duration, t_rules *rules);
long	ft_atoi(const char *str);
void	print_state(t_philo *philo, char *msg);
long	timestamp(void);
void	free_all(t_rules *rules);

#endif
