/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:30:37 by gapachec          #+#    #+#             */
/*   Updated: 2025/06/08 21:10:57 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Checks if a given string consists only of digits.
 * * @param str The string to check.
 * @return Returns 1 if the string contains only digits and is not empty, 0
 * otherwise.
 */
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

/**
 * @brief Validates the command-line arguments.
 * * Checks if the number of arguments is correct (5 or 6) and if all arguments
 * representing numerical values are valid numbers.
 * * @param argc The argument count.
 * @param argv The argument vector.
 * @return Returns 0 if arguments are valid, 1 if there's an error.
 */
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
