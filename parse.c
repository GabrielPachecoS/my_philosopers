/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:41:46 by gapachec          #+#    #+#             */
/*   Updated: 2025/09/03 01:58:46 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_digit_str(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

int	parse_args(int argc, char **argv, t_rules *rules)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!is_digit_str(argv[i]))
			return (printf("Error: invalid argument\n"), 1);
		i++;
	}
	rules->n_philos = ft_atoi(argv[1]);
	rules->t_die = ft_atoi(argv[2]);
	rules->t_eat = ft_atoi(argv[3]);
	rules->t_sleep = ft_atoi(argv[4]);
	rules->must_eat = -1;
	if (argc == 6)
		rules->must_eat = ft_atoi(argv[5]);
	rules->dead = 0;
	return (0);
}
