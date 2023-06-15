/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:13:32 by okraus            #+#    #+#             */
/*   Updated: 2023/05/12 18:19:03 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

int	ft_exec(t_pipex_info *info, int n)
{
	int		i;
	int		fail;
	char	*cmd;

	i = 0;
	fail = 1;
	while (info->paths[i])
	{
		cmd = ft_pathjoin(info->paths[i], info->args[n][0]);
		if (access(cmd, X_OK) == 0)
			execve(cmd, info->args[n], info->ev);
		free(cmd);
		i++;
	}
	ft_printf_fd(2, "%s: command not found\n", info->args[n][0]);
	return (fail);
}

int	ft_fail_exec(t_pipex_info *info, int n)
{
	ft_free_info(info);
	exit(n);
}

int	ft_args(t_pipex_info *info)
{
	int	i;

	i = 0;
	info->args = NULL;
	info->args = ft_calloc(info->ac - 2, sizeof(char **));
	while (i + 2 < info->ac - 1)
	{
		info->args[i] = ft_split(info->av[i + 2], ' ');
		i++;
	}
	info->arg = i;
	return (0);
}

//move to libft and test put strarray
int	ft_copy_strarray(int n, char **src, char ***dst)
{
	int	i;
	int	j;

	i = 0;
	dst[0] = ft_calloc((n + 1), sizeof(char *));
	if (!dst[0])
		return (1);
	while (i < n)
	{
		j = 0;
		dst[0][i] = ft_calloc((ft_strlen(src[i]) + 1), sizeof(char));
		if (!dst[0][i])
			return (2);
		while (src[i] && src[i][j])
		{
			dst[0][i][j] = src[i][j];
			j++;
		}
		dst[0][i][j] = '\0';
		i++;
	}
	dst[0][n] = NULL;
	return (0);
}

int	ft_dup(int newinput, int newoutput)
{
	if (dup2(newinput, STDIN_FILENO) < 0)
	{
		ft_printf_fd(2, "Error duplicating input\n");
		return (1);
	}
	if (dup2(newoutput, STDOUT_FILENO) < 0)
	{
		ft_printf_fd(2, "Error duplicating output\n");
		return (2);
	}
	return (0);
}
