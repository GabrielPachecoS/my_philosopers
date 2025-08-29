/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:51:11 by gapachec          #+#    #+#             */
/*   Updated: 2025/08/28 20:49:58 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Validates the command-line arguments.
 * * Checks if the number of arguments is correct (5 or 6) and if all arguments
 * representing numerical values are valid numbers.
 * * @param argc The argument count.
 * @param argv The argument vector.
 * @return Returns 0 if arguments are valid, 1 if there's an error.
 */
static int	start_threads(t_rules *rules)
{
	int	i;

	if (pthread_create(&rules->monitor_thread, NULL,
			&monitor_routine, rules) != 0)
		return (1);
	i = 0;
	while (i < rules->num_philo)
	{
		if (pthread_create(&rules->philos[i].thread_id, NULL, &philo_routine,
				&rules->philos[i]) != 0)
		{
			while (--i >= 0)
				pthread_join(rules->philos[i].thread_id, NULL);
			pthread_join(rules->monitor_thread, NULL);
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Handles the special case of a single philosopher.
 * * For a single philosopher, they think, try to take a fork (which is always
 * the same one), and then die because they can't take a second fork to eat.
 * * @param rules A pointer to the simulation rules structure.
 * @return Always returns 1 to indicate the scenario is handled and the program
 * can exit.
 */
static int	handle_one_philo(t_rules *rules)
{
	print_state(&rules->philos[0], "is thinking");
	print_state(&rules->philos[0], "has taken a fork");
	safe_usleep(rules->time_to_die, rules);
	print_state(&rules->philos[0], "died");
	free_all(rules);
	return (1);
}

/**
 * @brief Main function for the Philosophers problem simulation.
 * * Initializes rules, handles the single philosopher case, starts threads,
 * waits for all threads to finish, and cleans up resources.
 * * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 * @return Returns 0 on successful execution, 1 on error.
 */
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
	{
		free_all(&rules);
		return (printf("Error: failed to start threads\n"), 1);
	}
	i = 0;
	while (i < rules.num_philo)
	{
		if (pthread_join(rules.philos[i].thread_id, NULL) != 0)
		{
			free_all(&rules);
			return (printf("Error: failed to join philosopher thread\n"), 1);
		}
		i++;
	}
	if (pthread_join(rules.monitor_thread, NULL) != 0)
	{
		free_all(&rules);
		return (printf("Error: failed to join monitor thread\n"), 1);
	}
	free_all(&rules);
	return (0);
}
