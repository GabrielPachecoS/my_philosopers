/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:58:00 by gapachec          #+#    #+#             */
/*   Updated: 2025/09/03 01:58:33 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	philo_print(philo, "has taken a fork");
	usleep(philo->rules->t_die * 1000);
	pthread_mutex_lock(&philo->rules->death_mutex);
	if (!philo->rules->dead)
	{
		philo->rules->dead = 1;
		pthread_mutex_lock(&philo->rules->print_mutex);
		printf("%lld %d died\n", get_time_ms() - philo->rules->start_time,
			philo->id);
		pthread_mutex_unlock(&philo->rules->print_mutex);
	}
	pthread_mutex_unlock(&philo->rules->death_mutex);
	pthread_mutex_unlock(philo->left_fork);
}
