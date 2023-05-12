/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 15:42:22 by okraus            #+#    #+#             */
/*   Updated: 2023/05/12 18:11:36 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

static int	pipex(int argc, char *argv[], char *envp[])
{
	t_pipex_info	info;
	int				i;

	ft_get_info(&info, argc, argv, envp);
	ft_pipes(&info);
	i = 0;
	while (i < info.arg)
	{
		waitpid(info.pids[i], NULL, 0);
		i++;
	}
	ft_free_info(&info);
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc > 3)
	{
		pipex(argc, argv, envp);
	}
	else
	{
		ft_printf_fd(2, "Wrong number of arguments\n");
		return (1);
	}
	return (0);
}
