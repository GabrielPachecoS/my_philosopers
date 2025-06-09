/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/25 11:51:11 by gapachec          #+#    #+#             */
/* Updated: 2025/06/08 20:35:32 by gapachec         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Starts the philosopher and monitor threads.
 * * Creates the monitor thread first, then creates threads for each philosopher.
 * * @param rules A pointer to the simulation rules structure.
 * @return Returns 0 on success, 1 on failure to create any thread.
 */
static int	start_threads(t_rules *rules)
{
	int			i;

	// Create the monitor thread
	if (!pthread_create(&rules->monitor_thread, NULL, &monitor_routine, rules))
		return (1); // Return 1 if monitor thread creation fails
	i = 0;
	// Create threads for each philosopher
	while (i < rules->num_philo)
	{
		if (pthread_create(&rules->philos[i].thread_id, NULL, &philo_routine,
				&rules->philos[i]) != 0)
			return (1); // Return 1 if any philosopher thread creation fails
		i++;
	}
	return (0);
}

/**
 * @brief Handles the special case of a single philosopher.
 * * For a single philosopher, they think, try to take a fork (which is always the same one),
 * and then die because they can't take a second fork to eat.
 * * @param rules A pointer to the simulation rules structure.
 * @return Always returns 1 to indicate the scenario is handled and the program can exit.
 */
static int	handle_one_philo(t_rules *rules)
{
	print_state(&rules->philos[0], "is thinking"); // Philosopher starts thinking
	print_state(&rules->philos[0], "has taken a fork"); // Takes the only available fork
	safe_usleep(rules->time_to_die, rules); // Waits until time_to_die, simulating inability to eat
	print_state(&rules->philos[0], "died"); // Philosopher dies
	free_all(rules); // Clean up resources
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
	// Initialize simulation rules from command-line arguments
	if (init_rules(&rules, argc, argv) != 0)
		return (1); // Exit if rule initialization fails
	rules.time_initial = timestamp(); // Record the simulation start time
	if (rules.num_philo == 1) // Special case for a single philosopher
		return (handle_one_philo(&rules));
	// Start philosopher and monitor threads
	if (start_threads(&rules) != 0)
		return (printf("Error: failed to start threads\n"), 1); // Exit on thread creation error
	i = 0;
	// Wait for all philosopher threads to finish
	while (i < rules.num_philo)
	{
		if (pthread_join(rules.philos[i].thread_id, NULL) != 0)
			return (printf("Error: failed to join philosopher thread\n"), 1); // Exit on join error
		i++;
	}
	// Wait for the monitor thread to finish
	if (pthread_join(rules.monitor_thread, NULL) != 0)
		return (printf("Error: failed to join monitor thread\n"), 1); // Exit on join error
	free_all(&rules); // Free all allocated resources
	return (0);
}
