/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getinfo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:34:19 by okraus            #+#    #+#             */
/*   Updated: 2023/08/08 16:39:45 by okraus           ###   ########.fr       */
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
			ft_fail_exec(info, -1);
		}
		i++;
	}
	return (0);
}

int	ft_check_files(t_pipex_info *info)
{
	if (!info->hd)
	{
		if (!access(info->av[1], F_OK & R_OK))
		{
			info->fdi = open(info->av[1], O_RDONLY);
		}
		else
		{
			info->fdi = -1;
			perror(info->av[1]);
		}
	}
	if (!info->hd)
		info->fdo = open(info->av[info->ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		info->fdo = open(info->av[info->ac - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (access(info->av[info->ac - 1], F_OK | W_OK))
	{
		perror(info->av[info->ac - 1]);
		info->fdo = -1;
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
	info->hd = 0;
	if (!ft_strncmp("here_doc", info->av[1], 9))
	{
		info->fdi = 0;
		info->hd = 1;
	}
	ft_check_files(info);
	info->pids = ft_calloc(info->arg, sizeof(int));
	ft_open_pipes(info);
	return (0);
}

// typedef struct s_pipex_info
// {
// 	int		ac;			//argc
// 	int		fdi;		//fd of infile
// 	int		fdo;		//fd of outfile
//	int		arg;		//number of arguments
//	int		*pids;		//pids of child processes
//	int		**pipes;	//pipe fds
// 	char	**av;		//*argv[]
// 	char	**ev;		//*envp[]
// 	char	**paths;	//array of paths from envp
// 	char	***args;	//array of arrays of arguments
// }	t_pipex_info;

int	ft_free_info(t_pipex_info *info)
{
	int	i;

	i = 0;
	free(info->pids);
	ft_free_split(info->paths);
	while (info->args[i])
	{
		ft_free_split(info->args[i]);
		i++;
	}
	free(info->args);
	i = 0;
	while (i < info->arg - 1)
	{
		free(info->pipes[i]);
		i++;
	}
	free(info->pipes);
	return (0);
}
