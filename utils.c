/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:51:33 by gapachec          #+#    #+#             */
/*   Updated: 2025/06/08 20:38:21 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_state(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->rules->print_mutex);
	if (!philo->rules->someone_died)
	{
		printf("%ld %d %s\n", timestamp() - philo->rules->time_initial,
			philo->id, msg);
	}
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

long	ft_atoi(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

void	free_all(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philo)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		i++;
	}
	free(rules->forks);
	free(rules->philos);
	pthread_mutex_destroy(&rules->state_mutex);
	pthread_mutex_destroy(&rules->print_mutex);
}

void	safe_usleep(long duration, t_rules *rules)
{
	long	start;

	start = timestamp();
	while (!rules->someone_died && (timestamp() - start < duration))
		usleep(100);
}
