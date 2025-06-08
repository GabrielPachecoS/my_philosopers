/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:51:04 by gapachec          #+#    #+#             */
/*   Updated: 2025/06/08 20:33:24 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

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
