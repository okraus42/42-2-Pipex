/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 15:42:22 by okraus            #+#    #+#             */
/*   Updated: 2023/05/06 19:02:43 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

//get paths
void ft_fix_first_path(char **s)
{
	int		i;
	int		l;
	char	*str;

	i = 0;
	str = *s;
	l = ft_strlen(str);
	while (str[i + 5])
	{
		str[i] = str[i + 5];
		i++;
	}
	while (i < l)
	{
		str[i] = '\0';
		i++;
	}
}

static char	*ft_pathjoin(char *path, char *cmd)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = ft_calloc(ft_strlen(path) + 2 + ft_strlen(cmd), sizeof(char));
	while (path[i])
	{
		str[i] = path[i];
		i++;
	}
	str[i] = '/';
	i++;
	while (cmd[j])
	{
		str[i + j] = cmd[j];
		j++;
	}
	return (str);
}

static int	ft_test_exec(t_pipex_info *info)
{
	int		i;
	int		fail;
	char	*cmd;

	i = 0;
	fail = 1;
	info->args = ft_split(info->av[1], ' ');
	while (info->paths[i])
	{
		cmd = ft_pathjoin(info->paths[i], info->args[0]);
		execve(cmd, info->args, info->ev);
		free(cmd);
		i++;
	}
	ft_printf_fd(2, "Fail to run %s", info->av[1]);
	return (fail);
}

static int	paths(t_pipex_info *info)
{
	int	i;

	i = 0;
	info->paths = NULL;
	ft_printf("%i = %s\n", i, info->ev[5]);
	while (info->ev[i])
	{
		ft_printf("%i = %s\n", i, info->ev[i]);
		if (info->ev[i][0] == 'P' && info->ev[i][1] == 'A' && info->ev[i][2] == 'T'
			&& info->ev[i][3] == 'H' && info->ev[i][4] == '=')
		{
			ft_printf("envp %d = %s\n", i, info->ev[i]);
			info->paths = ft_split(info->ev[i], ':');
		}
		i++;
	}
	ft_fix_first_path(&info->paths[0]);
	return (0);
}

static int	ft_copy_strarray(int n, char **src, char ***dst)
{
	int	i;
	int	j;

	i = 0;
	dst[0] = ft_calloc((n + 1), sizeof(char*));
	if (!dst[0])
	{
		return (1);
	}
	while (i < n)
	{
		j = 0;
		dst[0][i] = ft_calloc((ft_strlen(src[i]) + 1), sizeof(char));
		if (!dst[0][i])
		{
			return (2);
		}
		while (src[i] && src[i][j])
		{
			//ft_printf("test5 %c %d %d\n", src[i][j], i, n);
			dst[0][i][j] = src[i][j];
			j++;
		}
		dst[0][i][j] = '\0';
		i++;
	}
	dst[0][n] = NULL;
	return (0);
}

void ft_put_strarray(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		ft_printf("arr%02i : %s\n", i, arr[i]);
		i++;
	}
}

static int	ft_get_info(t_pipex_info *info, int ac, char *av[], char *ev[])
{
	char	**avi;
	char	**evi;
	int		en;
	int		i;

	avi = NULL;
	evi = NULL;
	en = 0;
	info->ac = ac;
	//ft_put_strarray(av);
	ft_copy_strarray(ac, av, &avi);
	//ft_put_strarray(avi);
	info->av = avi;
	while (ev[en])
	{
		//ft_printf("%i:%s\n", en, ev[en]);
		en++;
	}
	ft_copy_strarray(en, ev, &evi);
	info->ev = evi;
	ft_put_strarray(info->ev);
	ft_put_strarray(evi);
	paths(info);
	i = 0;
	while (info->paths && info->paths[i])
	{
		ft_printf("path %i = %s\n", i, info->paths[i]);
		i++;
	}
	ft_test_exec(info);
	return (0);
}

static int	pipex(int argc, char *argv[], char *envp[])
{
	t_pipex_info info;
	int	i;

	ft_get_info(&info, argc, argv, envp);
	return (0);
}

//call split if only one argument is passed to the function,
//else call the other function
int	main(int argc, char *argv[], char *envp[])
{
	if (argc == 2)
	{
		ft_printf("cmd = %s\n", argv[1]);
		pipex(argc, argv, envp);
	}
	else
	{
		ft_printf_fd(2, "Wrong number of arguments\n");
		return (1);
	}
	return (0);
}
