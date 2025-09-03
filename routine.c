/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:48:34 by gapachec          #+#    #+#             */
/*   Updated: 2025/09/03 01:59:38 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	philo_print(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	philo_print(philo, "has taken a fork");
	philo_print(philo, "is eating");
	philo->last_meal = get_time_ms();
	usleep(philo->rules->t_eat * 1000);
	philo->meal_count++;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	philo_sleep(t_philo *philo)
{
	philo_print(philo, "is sleeping");
	usleep(philo->rules->t_sleep * 1000);
}

void	philo_think(t_philo *philo)
{
	philo_print(philo, "is thinking");
}

void	philo_loop(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->rules->death_mutex);
		if (philo->rules->dead)
		{
			pthread_mutex_unlock(&philo->rules->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->rules->death_mutex);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	if (philo->rules->n_philos == 1)
	{
		handle_one_philo(philo);
		return (NULL);
	}
	philo_loop(philo);
	return (NULL);
}
