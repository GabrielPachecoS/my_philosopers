/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:55:13 by gapachec          #+#    #+#             */
/*   Updated: 2025/06/08 20:39:48 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_died(t_philo *philo, long time)
{
	printf("%ld %d died\n", time, philo->id);
	return (1);
}

static int	check_death(t_rules *rules)
{
	int		i;
	long	time;
	long	time_snc_lst_meal;

	i = 0;
	while (i < rules->num_philo)
	{
		pthread_mutex_lock(&rules->state_mutex);
		time = timestamp() - rules->time_initial;
		time_snc_lst_meal = time - rules->philos[i].last_meal_time;
		if (time_snc_lst_meal >= rules->time_to_die)
		{
			rules->someone_died = 1;
			pthread_mutex_lock(&rules->print_mutex);
			philo_died(&rules->philos[i], time);
			pthread_mutex_unlock(&rules->print_mutex);
			pthread_mutex_unlock(&rules->state_mutex);
			return (1);
		}
		pthread_mutex_unlock(&rules->state_mutex);
		i++;
	}
	return (0);
}

static int	all_philos_ate(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philo)
	{
		if (rules->philos[i].meals_eaten < rules->must_eat_count)
			return (0);
		i++;
	}
	return (1);
}

static int	check_all_ate(t_rules *rules)
{
	if (rules->must_eat_count <= 0)
		return (0);
	if (all_philos_ate(rules))
	{
		pthread_mutex_lock(&rules->state_mutex);
		rules->someone_died = 1;
		pthread_mutex_unlock(&rules->state_mutex);
		pthread_mutex_lock(&rules->print_mutex);
		printf("%ld All philosophers have eaten enough\n", \
			timestamp() - rules->time_initial);
		pthread_mutex_unlock(&rules->print_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_rules	*rules;

	rules = arg;
	while (1)
	{
		if (check_death(rules))
			return (NULL);
		if (check_all_ate(rules))
			return (NULL);
		usleep(1000);
	}
}
