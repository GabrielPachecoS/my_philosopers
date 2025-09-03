/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:22:21 by gapachec          #+#    #+#             */
/*   Updated: 2025/09/03 01:58:19 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_philo_death(t_rules *rules)
{
	int			i;
	long long	time_since_last_meal;

	i = 0;
	while (i < rules->n_philos)
	{
		time_since_last_meal = get_time_ms() - rules->philos[i].last_meal;
		if (time_since_last_meal >= rules->t_die)
		{
			pthread_mutex_lock(&rules->death_mutex);
			if (!rules->dead)
			{
				rules->dead = 1;
				pthread_mutex_lock(&rules->print_mutex);
				printf("%lld %d died\n", get_time_ms() - rules->start_time,
					rules->philos[i].id);
				pthread_mutex_unlock(&rules->print_mutex);
			}
			pthread_mutex_unlock(&rules->death_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_all_ate(t_rules *rules)
{
	int	i;

	if (rules->must_eat == -1)
		return (0);
	i = 0;
	while (i < rules->n_philos)
	{
		if (rules->philos[i].meal_count < rules->must_eat)
			return (0);
		i++;
	}
	pthread_mutex_lock(&rules->death_mutex);
	rules->dead = 1;
	pthread_mutex_unlock(&rules->death_mutex);
	return (1);
}

void	*monitor_death(void *arg)
{
	t_rules	*rules;

	rules = (t_rules *)arg;
	while (1)
	{
		if (check_philo_death(rules))
			return (NULL);
		if (check_all_ate(rules))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
