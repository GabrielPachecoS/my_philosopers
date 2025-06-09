/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* utils.c                                            :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/04/25 11:51:33 by gapachec          #+#    #+#             */
/* Updated: 2025/06/08 20:38:21 by gapachec         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Retrieves the current time in milliseconds.
 * * Uses `gettimeofday` to get the current time and converts it to milliseconds.
 * * @return The current time in milliseconds since the Epoch.
 */
long	timestamp(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL); // Get current time
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)); // Convert to milliseconds
}

/**
 * @brief Prints the current state of a philosopher.
 * * This function locks a mutex to ensure that state messages are printed
 * without interleaving from other threads, preventing race conditions on stdout.
 * The message is only printed if no philosopher has died.
 * * @param philo A pointer to the philosopher structure.
 * @param msg The string message to be printed (e.g., "is eating", "is sleeping").
 */
void	print_state(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->rules->print_mutex); // Lock mutex before printing
	if (!philo->rules->someone_died) // Only print if no one has died yet
	{
		printf("%ld %d %s\n", timestamp() - philo->rules->time_initial,
			philo->id, msg);
	}
	pthread_mutex_unlock(&philo->rules->print_mutex); // Unlock mutex after printing
}

/**
 * @brief Converts a string to a long integer.
 * * This is a custom `atoi` implementation, handling leading whitespace and an optional sign.
 * * @param str The string to convert.
 * @return The converted long integer value.
 */
long	ft_atoi(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	// Skip leading whitespace characters
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	// Handle optional sign
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	// Convert digits to long integer
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

/**
 * @brief Frees all dynamically allocated memory and destroys mutexes.
 * * This function is responsible for cleaning up resources, including
 * forks (mutexes), philosopher structures, and the global mutexes.
 * * @param rules A pointer to the simulation rules structure.
 */
void	free_all(t_rules *rules)
{
	int	i;

	i = 0;
	// Destroy all fork mutexes
	while (i < rules->num_philo)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		i++;
	}
	free(rules->forks); // Free memory allocated for forks
	free(rules->philos); // Free memory allocated for philosophers
	pthread_mutex_destroy(&rules->state_mutex); // Destroy state mutex
	pthread_mutex_destroy(&rules->print_mutex); // Destroy print mutex
}

/**
 * @brief A safe version of `usleep` that can be interrupted if a philosopher dies.
 * * This function repeatedly sleeps for small intervals, checking the `someone_died`
 * flag to allow for early termination of the sleep if a death occurs.
 * * @param duration The desired sleep duration in milliseconds.
 * @param rules A pointer to the simulation rules structure, used to check `someone_died` flag.
 */
void	safe_usleep(long duration, t_rules *rules)
{
	long	start;

	start = timestamp(); // Record start time
	// Continue sleeping until duration passed or someone died
	while (!rules->someone_died && (timestamp() - start < duration))
		usleep(100); // Sleep in 100 microsecond increments
}
