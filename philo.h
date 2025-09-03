/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:38:32 by gapachec          #+#    #+#             */
/*   Updated: 2025/09/03 01:59:15 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_rules
{
	int				n_philos;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				must_eat;
	int				dead;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	t_philo			*philos;
	pthread_t		*threads;
	pthread_t		monitor;
}	t_rules;

struct s_philo
{
	int				id;
	int				meal_count;
	long long		last_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_rules			*rules;
};

int			parse_args(int argc, char **argv, t_rules *rules);
int			init_all(t_rules *rules);
int			start_simulation(t_rules *rules);
void		cleanup(t_rules *rules);

void		*philo_routine(void *arg);
void		*monitor_death(void *arg);
void		philo_print(t_philo *philo, char *msg);
int			ft_atoi(const char *str);
int			is_digit_str(char *str);
long long	get_time_ms(void);
void		handle_one_philo(t_philo *philo);
void		philo_eat(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		philo_think(t_philo *philo);
void		philo_loop(t_philo *philo);

#endif
