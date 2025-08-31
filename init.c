/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:51:40 by gapachec          #+#    #+#             */
/*   Updated: 2025/08/31 15:10:36 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Initializes the core rule values from command-line arguments.
 * * Converts string arguments to long integers and sets the initial values
 * for the simulation rules. `must_eat_count` is set to -1 by default if not
 * provided.
 * * @param rules A pointer to the `t_rules` structure to be initialized.
 * @param argc The argument count.
 * @param argv The argument vector.
 */
static void	init_rules_values(t_rules *rules, int argc, char **argv)
{
	rules->num_philo = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	rules->must_eat_count = -1;
	rules->someone_died = 0;
	if (argc == 6)
		rules->must_eat_count = ft_atoi(argv[5]);
}

/**
 * @brief Initializes all mutexes required for the simulation.
 * * This includes the state mutex, print mutex, and an array of mutexes for
 * the forks. Dynamically allocates memory for the forks array.
 * * @param rules A pointer to the `t_rules` structure.
 * @return Returns 0 on success, 1 on memory allocation or mutex initialization
 * failure.
 */
static int	init_mutexes(t_rules *rules)
{
	int	i;

	pthread_mutex_init(&rules->state_mutex, NULL);
	pthread_mutex_init(&rules->print_mutex, NULL);
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philo);
	if (!rules->forks)
		return (1);
	i = 0;
	while (i < rules->num_philo)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Initializes each philosopher's structure.
 * * Allocates memory for the array of `t_philo` structures and sets
 * initial values for each philosopher, including their ID, meal count,
 * last meal time, and fork assignments.
 * * @param rules A pointer to the `t_rules` structure.
 * @return Returns 0 on success, 1 on memory allocation failure.
 */
static int	init_philos(t_rules *rules)
{
	int	i;

	rules->philos = malloc(sizeof(t_philo) * rules->num_philo);
	if (!rules->philos)
		return (1);
	i = 0;
	while (i < rules->num_philo)
	{
		rules->philos[i].id = i + 1;
		rules->philos[i].meals_eaten = 0;
		rules->philos[i].last_meal_time = timestamp();
		rules->philos[i].left_fork_id = i;
		rules->philos[i].right_fork_id = (i + 1) % rules->num_philo;
		rules->philos[i].rules = rules;
		i++;
	}
	return (0);
}

/**
 * @brief Main initialization function for the simulation rules and entities.
 * * Orchestrates the argument validation, rule value setting, mutex 
 * initialization, and philosopher initialization.
 * * @param rules A pointer to the `t_rules` structure to be initialized.
 * @param argc The argument count.
 * @param argv The argument vector.
 * @return Returns 0 on complete success, 1 if any initialization step fails.
 */
int	init_rules(t_rules *rules, int argc, char **argv)
{
	if (check_args_validity(argc, argv) != 0)
		return (1);
	init_rules_values(rules, argc, argv);
	if (init_mutexes(rules) != 0)
	{
		free_all(rules);
		return (1);
	}
	if (init_philos(rules) != 0)
	{
		free_all(rules);
		return (1);
	}
	return (0);
}
