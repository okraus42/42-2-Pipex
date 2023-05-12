/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 15:55:55 by okraus            #+#    #+#             */
/*   Updated: 2023/05/12 18:18:02 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

// void	here_doc(t_pipex *pipex, char **av)
// {
// 	char	*line;

// 	line = NULL;
// 	while (1)
// 	{
// 		write (0, "here_doc>", 9);
// 		line = get_next_line(0);
// 		if (line == NULL)
// 			return ; // ERROR
// 		if (ft_strncmp(line, av[2], ft_strlen(av[2])) == 0)
// 			break ;
// 		write(pipex->pipe[1], &line, ft_strlen(line));
// 		free (line);
// 	}
// 	free (line);
// 	pipex->infile_fd = pipex->pipe[0];
// }

void	ft_heredoc(t_pipex_info *info, int i)
{
	char	*line;

	line = NULL;
	info->pids[i] = fork();
	if (info->pids[i] == -1)
		ft_fail_exec(info, -9);
	if (info->pids[i] == 0)
	{
		while (1)
		{
			write (0, "> ", 3);
			line = get_next_line(0);
			if (line == NULL)
				ft_fail_exec(info, -2);
			if (ft_strncmp(line, info->av[2], ft_strlen(info->av[2])) == 0)
				break ;
			write(info->pipes[0][1], line, ft_strlen(line));
			free(line);
		}
		close(info->pipes[0][1]);
		close(info->pipes[0][0]);
		free(line);
		ft_fail_exec(info, i);
	}
}

int	ft_firstpipe(t_pipex_info *info, int i, int j)
{
	info->pids[i] = fork();
	if (info->pids[i] == -1)
	{
		ft_printf_fd(2, "Error with creating process\n");
		return (2);
	}
	if (info->pids[i] == 0)
	{
		if (info->fdi == -1)
			ft_fail_exec(info, i);
		if (ft_dup(info->fdi, info->pipes[i][1]))
			return (3);
		close (info->fdi);
		j = 0;
		while (j < info->arg - 1)
		{
			close(info->pipes[j][0]);
			close(info->pipes[j][1]);
			j++;
		}
		if (ft_exec(info, i))
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
			if (ft_exec(info, i))
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
		if (info->fdo == -1)
			ft_fail_exec(info, i);
		j = 0;
		if (ft_dup(info->pipes[i - 1][0], info->fdo))
			return (3);
		close (info->fdo);
		while (j < info->arg - 1)
		{
			close(info->pipes[j][0]);
			close(info->pipes[j][1]);
			j++;
		}
		if (ft_exec(info, i))
			ft_fail_exec(info, i);
	}
	return (0);
}

int	ft_pipes(t_pipex_info *info)
{
	int	i;
	int	j;

	i = 0;
	if (!info->hd)
		ft_firstpipe(info, i, j);
	else
		ft_heredoc(info, i);
	i = 1;
	ft_middlepipes(info, i, j);
	ft_lastpipe(info, i, j);
	i = 0;
	close (info->fdi);
	close (info->fdo);
	while (i < info->arg - 1)
	{
		close(info->pipes[i][0]);
		close(info->pipes[i][1]);
		i++;
	}
	return (0);
}
