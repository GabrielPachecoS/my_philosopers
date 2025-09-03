/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 08:41:11 by gapachec          #+#    #+#             */
/*   Updated: 2025/09/03 02:11:17 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_simulation(t_rules *rules)
{
	int			i;

	rules->start_time = get_time_ms();
	i = 0;
	while (i < rules->n_philos)
	{
		rules->philos[i].last_meal = rules->start_time;
		if (pthread_create(&rules->threads[i], NULL,
				philo_routine, &rules->philos[i]))
			return (printf("Error: thread create failed\n"), 1);
		i++;
	}
	if (pthread_create(&rules->monitor, NULL, monitor_death, rules))
		return (printf("Error: monitor thread failed\n"), 1);
	i = 0;
	while (i < rules->n_philos)
		pthread_join(rules->threads[i++], NULL);
	pthread_join(rules->monitor, NULL);
	return (0);
}
