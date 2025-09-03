/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:40:29 by gapachec          #+#    #+#             */
/*   Updated: 2025/09/03 01:57:47 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->n_philos)
	{
		rules->philos[i].id = i + 1;
		rules->philos[i].meal_count = 0;
		rules->philos[i].last_meal = get_time_ms();
		rules->philos[i].rules = rules;
		rules->philos[i].left_fork = &rules->forks[i];
		rules->philos[i].right_fork = &rules->forks[(i + 1) % rules->n_philos];
		i++;
	}
}

int	init_all(t_rules *rules)
{
	int	i;

	rules->philos = malloc(sizeof(t_philo) * rules->n_philos);
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->n_philos);
	rules->threads = malloc(sizeof(pthread_t) * rules->n_philos);
	if (!rules->philos || !rules->forks || !rules->threads)
		return (printf("Error: malloc failed\n"), 1);
	i = 0;
	while (i < rules->n_philos)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL))
			return (printf("Error: mutex init failed\n"), 1);
		i++;
	}
	if (pthread_mutex_init(&rules->print_mutex, NULL)
		|| pthread_mutex_init(&rules->death_mutex, NULL))
		return (printf("Error: mutex init failed\n"), 1);
	assign_forks(rules);
	return (0);
}
