/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 15:55:55 by okraus            #+#    #+#             */
/*   Updated: 2023/05/10 15:58:45 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

int	ft_firstpipe(t_pipex_info *info, int i, int j)
{
	i = 0;
	info->pids[i] = fork();
	if (info->pids[i] == -1)
	{
		ft_printf_fd(2, "Error with creating process\n");
		return (2);
	}
	if (info->pids[i] == 0)
	{
		if (ft_dup(info->fdi, info->pipes[i][1]))
			return (3);
		j = 0;
		while (j < info->arg - 1)
		{
			close(info->pipes[j][0]);
			close(info->pipes[j][1]);
			j++;
		}
		if (ft_test_exec(info, i))
			ft_fail_exec(info, i);
	}
	return (0);
}

int	ft_middlepipes(t_pipex_info *info, int i, int j)
{
	while (i < info->arg - 1)
	{
		info->pids[i] = fork();
		if (info->pids[i] == -1)
		{
			ft_printf_fd(2, "Error with creating process\n");
			return (2);
		}
		if (info->pids[i] == 0)
		{
			if (ft_dup(info->pipes[i - 1][0], info->pipes[i][1]))
				return (3);
			j = 0;
			while (j < info->arg - 1)
			{
				close(info->pipes[j][0]);
				close(info->pipes[j][1]);
				j++;
			}
			if (ft_test_exec(info, i))
				ft_fail_exec(info, i);
		}
		i++;
	}
	return (0);
}

int	ft_lastpipe(t_pipex_info *info, int i, int j)
{
	i = info->arg - 1;
	info->pids[i] = fork();
	if (info->pids[i] == -1)
	{
		ft_printf_fd(2, "Error with creating process\n");
		return (2);
	}
	if (info->pids[i] == 0)
	{
		j = 0;
		if (ft_dup(info->pipes[i - 1][0], info->fdo))
			return (3);
		while (j < info->arg - 1)
		{
			close(info->pipes[j][0]);
			close(info->pipes[j][1]);
			j++;
		}
		if (ft_test_exec(info, i))
			ft_fail_exec(info, i);
	}
	return (0);
}

int	ft_pipes(t_pipex_info *info)
{
	int	i;
	int	j;

	ft_firstpipe(info, i, j);
	i = 1;
	ft_middlepipes(info, i, j);
	ft_lastpipe(info, i, j);
	j = 0;
	while (j < info->arg - 1)
	{
		close(info->pipes[j][0]);
		close(info->pipes[j][1]);
		j++;
	}
	i = 0;
	while (i < info->arg)
	{
		waitpid(info->pids[i], NULL, 0);
		i++;
	}
	return (0);
}
