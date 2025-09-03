/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapachec <gapachec@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:38:40 by gapachec          #+#    #+#             */
/*   Updated: 2025/09/03 01:57:00 by gapachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_rules	rules;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo n_philo t_die t_eat t_sleep [must_eat]\n");
		return (1);
	}
	if (parse_args(argc, argv, &rules))
		return (1);
	if (init_all(&rules))
	{
		cleanup(&rules);
		return (1);
	}
	if (start_simulation(&rules))
	{
		cleanup(&rules);
		return (1);
	}
	cleanup(&rules);
	return (0);
}
