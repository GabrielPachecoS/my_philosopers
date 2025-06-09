/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* philo_actions.c                                    :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/25 12:34:22 by gapachec          #+#    #+#             */
/* Updated: 2025/04/25 15:55:15 by gapachec         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Simulates a philosopher thinking.
 * * Prints the "is thinking" state message for the given philosopher.
 * * @param philo A pointer to the philosopher structure.
 */
static void	philo_think(t_philo *philo)
{
	print_state(philo, "is thinking");
}

/**
 * @brief Simulates a philosopher eating.
 * * Philosophers pick up forks (left then right for odd IDs, right then left for even IDs),
 * update their last meal time, eat for `time_to_eat`, and then put down the forks.
 * Their meal count is incremented.
 * * @param philo A pointer to the philosopher structure.
 */
static void	philo_eat(t_philo *philo)
{
	// Acquire forks based on philosopher ID to prevent deadlock
	if (philo->id % 2 == 0) // Even ID philosophers pick up right then left
	{
		pthread_mutex_lock(&philo->rules->forks[philo->right_fork_id]);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&philo->rules->forks[philo->left_fork_id]);
		print_state(philo, "has taken a fork");
	}
	else // Odd ID philosophers pick up left then right
	{
		pthread_mutex_lock(&philo->rules->forks[philo->left_fork_id]);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&philo->rules->forks[philo->right_fork_id]);
		print_state(philo, "has taken a fork");
	}
	pthread_mutex_lock(&philo->rules->state_mutex); // Lock state mutex to update last_meal_time safely
	philo->last_meal_time = timestamp(); // Update last meal time
	pthread_mutex_unlock(&philo->rules->state_mutex); // Unlock state mutex
	print_state(philo, "is eating"); // Announce eating
	safe_usleep(philo->rules->time_to_eat, philo->rules); // Simulate eating duration
	pthread_mutex_unlock(&philo->rules->forks[philo->left_fork_id]); // Release left fork
	pthread_mutex_unlock(&philo->rules->forks[philo->right_fork_id]); // Release right fork
	philo->meals_eaten++; // Increment meal count
}

/**
 * @brief Simulates a philosopher sleeping.
 * * Prints the "is sleeping" state message and pauses the philosopher's thread
 * for `time_to_sleep` duration.
 * * @param philo A pointer to the philosopher structure.
 */
static void	philo_sleep(t_philo *philo)
{
	print_state(philo, "is sleeping"); // Announce sleeping
	safe_usleep(philo->rules->time_to_sleep, philo->rules); // Simulate sleeping duration
}

/**
 * @brief The main routine for each philosopher thread.
 * * Philosophers continuously loop through thinking, eating, and sleeping actions
 * until the simulation is terminated (e.g., by death or all meals eaten).
 * An initial `usleep` is added for even-indexed philosophers to help prevent immediate deadlocks.
 * * @param arg A void pointer to a `t_philo` structure.
 * @return Returns NULL when the philosopher's routine ends.
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// Introduce a small delay for even-indexed philosophers to avoid immediate deadlock
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1) // Main philosopher action loop
	{
		if (philo->rules->someone_died) // Check if simulation should terminate due to death
			break ;
		philo_think(philo); // Philosopher thinks
		if (philo->rules->someone_died) // Re-check after thinking
			break ;
		philo_eat(philo); // Philosopher eats
		if (philo->rules->someone_died) // Re-check after eating
			break ;
		// Check if philosopher has eaten enough and simulation should terminate
		if (philo->rules->must_eat_count > 0
			&& philo->meals_eaten >= philo->rules->must_eat_count)
			break ;
		philo_sleep(philo); // Philosopher sleeps
	}
	return (NULL);
}
