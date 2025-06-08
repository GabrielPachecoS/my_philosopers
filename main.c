/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:51:11 by gapachec          #+#    #+#             */
/*   Updated: 2025/06/08 20:35:32 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start_threads(t_rules *rules)
{
	int			i;

	if (!pthread_create(&rules->monitor_thread, NULL, &monitor_routine, rules))
		return (1);
	i = 0;
	while (i < rules->num_philo)
	{
		if (pthread_create(&rules->philos[i].thread_id, NULL, &philo_routine,
				&rules->philos[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

static int	handle_one_philo(t_rules *rules)
{
	print_state(&rules->philos[0], "is thinking");
	print_state(&rules->philos[0], "has taken a fork");
	safe_usleep(rules->time_to_die, rules);
	print_state(&rules->philos[0], "died");
	free_all(rules);
	return (1);
}

int	main(int argc, char **argv)
{
	t_rules	rules;
	int		i;

	i = 0;
	if (init_rules(&rules, argc, argv) != 0)
		return (1);
	rules.time_initial = timestamp();
	if (rules.num_philo == 1)
		return (handle_one_philo(&rules));
	if (start_threads(&rules) != 0)
		return (printf("Error: failed to start threads\n"), 1);
	i = 0;
	while (i < rules.num_philo)
	{
		if (pthread_join(rules.philos[i].thread_id, NULL) != 0)
			return (printf("Error: failed to join philosopher thread\n"), 1);
		i++;
	}
	if (pthread_join(rules.monitor_thread, NULL) != 0)
		return (printf("Error: failed to join monitor thread\n"), 1);
	free_all(&rules);
	return (0);
}
