/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:55:13 by gapachec          #+#    #+#             */
/*   Updated: 2025/06/08 21:07:18 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Prints a message indicating a philosopher has died.
 * * @param philo A pointer to the philosopher who died.
 * @param time The current timestamp when the philosopher died.
 * @return Returns 1, indicating a death has occurred.
 */
static int	philo_died(t_philo *philo, long time)
{
	printf("%ld %d died\n", time, philo->id);
	return (1);
}

/**
 * @brief Checks if any philosopher has died.
 * * Iterates through all philosophers and checks if the time since
 * their last meal exceeds the `time_to_die` threshold. If a 
 * philosopher has died, it updates the `someone_died` flag in the
 * rules and prints a death message.
 * * @param rules A pointer to the simulation rules structure.
 * @return Returns 1 if a philosopher has died, 0 otherwise.
 */
static int	check_death(t_rules *rules)
{
	int		i;
	long	time;
	long	time_snc_lst_meal;

	i = 0;
	while (i < rules->num_philo)
	{
		pthread_mutex_lock(&rules->state_mutex);
		time = timestamp() - rules->time_initial;
		time_snc_lst_meal = time - rules->philos[i].last_meal_time;
		if (time_snc_lst_meal >= rules->time_to_die)
		{
			rules->someone_died = 1;
			pthread_mutex_lock(&rules->print_mutex);
			philo_died(&rules->philos[i], time);
			pthread_mutex_unlock(&rules->print_mutex);
			pthread_mutex_unlock(&rules->state_mutex);
			return (1);
		}
		pthread_mutex_unlock(&rules->state_mutex);
		i++;
	}
	return (0);
}

/**
 * @brief Checks if all philosophers have eaten the required number of meals.
 * * @param rules A pointer to the simulation rules structure.
 * @return Returns 1 if all philosophers have eaten enough, 0 otherwise.
 */
static int	all_philos_ate(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philo)
	{
		if (rules->philos[i].meals_eaten < rules->must_eat_count)
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Checks if the simulation should end because all philosophers have
 * eaten enough.
 * * If `must_eat_count` is set and all philosophers have met this requirement,
 * the `someone_died` flag is set to terminate the simulation and a message is
 * printed.
 * * @param rules A pointer to the simulation rules structure.
 * @return Returns 1 if all philosophers have eaten enough and the simulation
 * should end, 0 otherwise.
 */
static int	check_all_ate(t_rules *rules)
{
	if (rules->must_eat_count <= 0)
		return (0);
	if (all_philos_ate(rules))
	{
		pthread_mutex_lock(&rules->state_mutex);
		rules->someone_died = 1;
		pthread_mutex_unlock(&rules->state_mutex);
		pthread_mutex_lock(&rules->print_mutex);
		printf("%ld All philosophers have eaten enough\n", \
			timestamp() - rules->time_initial);
		pthread_mutex_unlock(&rules->print_mutex);
		return (1);
	}
	return (0);
}

/**
 * @brief The main routine for the monitoring thread.
 * * Continuously checks for two conditions: if any philosopher has died, or
 * if all philosophers have eaten the required number of meals. The routine
 * exits if either condition is met.
 * * @param arg A void pointer to the `t_rules` structure.
 * @return Returns NULL when the monitoring ends.
 */
void	*monitor_routine(void *arg)
{
	t_rules	*rules;

	rules = arg;
	while (1)
	{
		if (check_death(rules))
			return (NULL);
		if (check_all_ate(rules))
			return (NULL);
		usleep(1000);
	}
}
