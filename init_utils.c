/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:30:37 by gapachec          #+#    #+#             */
/*   Updated: 2025/06/08 20:36:13 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_valid_number(const char *str)
{
	if (!*str)
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int	check_args_validity(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (printf("Error: invalid number of arguments\n"), 1);
	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
			return (printf("Error: invalid argument '%s'\n", argv[i]), 1);
		i++;
	}
	return (0);
}
