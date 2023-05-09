/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getinfo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:34:19 by okraus            #+#    #+#             */
/*   Updated: 2023/05/09 16:36:04 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

int	ft_open_pipes(t_pipex_info *info)
{
	int	i;

	info->pipes = ft_calloc(info->arg - 1, sizeof(int *));
	i = 0;
	while (i < info->arg - 1)
	{
		info->pipes[i] = ft_calloc(sizeof(int), 2);
		i++;
	}
	i = 0;
	while (i < info->arg - 1)
	{
		if (pipe(info->pipes[i]) == -1)
		{
			ft_printf_fd(2, "Error with creating pipe\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_get_info(t_pipex_info *info, int ac, char *av[], char *ev[])
{
	info->ac = ac;
	info->av = av;
	info->ev = ev;
	ft_paths(info);
	ft_args(info);
	//check here_doc
	info->fdi = open(info->av[1], O_RDONLY);
	info->fdo = open(info->av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	info->pids = ft_calloc(info->arg, sizeof(int));
	ft_open_pipes(info);
	return (0);
}
