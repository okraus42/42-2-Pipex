/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 15:42:22 by okraus            #+#    #+#             */
/*   Updated: 2023/05/08 15:42:45 by okraus           ###   ########.fr       */
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

static int	ft_test_exec(t_pipex_info *info, int n)
{
	int		i;
	int		fail;
	char	*cmd;

	ft_printf_fd(2, "Running ft_test_exec\n");
	i = 0;
	fail = 1;
	//info->args = ft_split(info->av[1], ' ');
	while (info->paths[i])
	{
		cmd = ft_pathjoin(info->paths[i], info->args[n][0]);
		execve(cmd, info->args[n], info->ev);
		free(cmd);
		i++;
	}
	ft_printf_fd(2, "Fail to run %s\n", info->av[1]);
	return (fail);
}

static int	ft_pipes(t_pipex_info *info)
{
	int	i;
	int	j;

	ft_printf("Running ft_pipes\n");
	info->pids = ft_calloc(info->arg, sizeof(int));
	info->pipes = ft_calloc(info->arg - 1, sizeof(int*));
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
	i = 0;
	info->pids[i] = fork();
	if (info->pids[i] == -1)
	{
		ft_printf_fd(2, "Error with creating process\n");
		return (2);
	}
	// Child process to handle infile
	if (info->pids[i] == 0)
	{
		//code
		if (dup2(info->fdi, STDIN_FILENO) < 0) //STDIN_FILENO = 0;
		{
			ft_printf_fd(2, "Error 00");
		}
		//close unnecessary pipes
		j = 0;
		
		while (j < info->arg - 1)
		{
			if (j != i - 1)
			{
				close(info->pipes[j][0]);
			}
			if (j != i)
			{
				close(info->pipes[j][1]);
			}
			j++;
		}
		if (dup2(info->pipes[i][1], STDOUT_FILENO) < 0) // STDOUT_FILENO = 1;
		{
			ft_printf_fd(2, "Error 00");
		}
		close(info->pipes[0][1]);
		//close file descriptor???
		ft_test_exec(info, i);
	}
	// super loop for the middle pipes
	i = 1;
	while (i < info->arg - 1)
	{
		sleep(1);
		info->pids[i] = fork();
		if (info->pids[i] == -1)
		{
			ft_printf_fd(2, "Error with creating process\n");
			return (2);
		}
		if (info->pids[i] == 0)
		{
			ft_printf("Hello from child %i\n", i);
			if (dup2(info->pipes[i - 1][0], STDIN_FILENO) < 0) //STDIN_FILENO = 0;
			{
				ft_printf_fd(2, "Error 00");
			}
			if (dup2(info->pipes[i][1], STDOUT_FILENO) < 0) // STDOUT_FILENO = 1;
			{
				ft_printf_fd(2, "Error 00");
			}
			close(info->pipes[i][1]);
			close(info->pipes[i - 1][0]);
			//close unnecessary pipes
			j = 0;
			
			while (j < info->arg - 1)
			{
				if (j != i - 1)
				{
					close(info->pipes[j][0]);
				}
				if (j != i)
				{
					close(info->pipes[j][1]);
				}
				j++;
			}
			ft_test_exec(info, i);
		}
		ft_printf("Hello from parent after creating child %i\n", i);
		////END OF LOOP
		i++;
	}
	// Child process to handle outfile
	i = info->arg - 1;
	info->pids[i] = fork();
	if (info->pids[i] == -1)
	{
		ft_printf_fd(2, "Error with creating process\n");
		return (2);
	}
	if (info->pids[i] == 0)
	{
		sleep(2);
		//code
		// ft_printf("Process%i goes to sleep\n", i);
		// sleep(4);
		// ft_printf("Process%i wakes up\n", i);
		//close unnecessary pipes
		j = 0;
		
		while (j < info->arg - 1)
		{
			if (j != i - 1)
			{
				close(info->pipes[j][0]);
			}
			if (j != i)
			{
				close(info->pipes[j][1]);
			}
			j++;
		}
		if (dup2(info->pipes[i - 1][0], STDIN_FILENO) < 0) //STDIN_FILENO = 0;
		{
			ft_printf_fd(2, "Error 00");
		}
		close(info->pipes[i - 1][0]);
		dup2(info->fdo, STDOUT_FILENO); //STDOUT_FILENO = 1;
		//close file descriptor???
		// ft_printf("Process%i goes to sleep\n", i);
		// sleep(4);
		// ft_printf("Process%i wakes up\n", i);
		ft_test_exec(info, i);
	}
	ft_printf("Hello from parent near end %i\n", i);
	// MAIN PROCESS
	j = 0;
	while (j < info->arg - 1)
	{
		close(info->pipes[j][0]);
		close(info->pipes[j][1]);
		j++;
	}
	i = 0;
	sleep(5);
	ft_printf_fd(2, "waited for %i", i);
	while (i < info->arg)
	{
		waitpid(info->pids[i], NULL, 0); //waitpid would be better?
		i++;
		ft_printf_fd(2, "waited for %i", i);
	}
	return (0);
}

static int	ft_args(t_pipex_info *info)
{
	int	i;

	ft_printf("Running ft_args\n");
	i = 0;
	info->args = NULL;
	info->args = ft_calloc(info->ac - 2, sizeof(char**));
	while (i + 2 < info->ac - 1) // why -1??
	{
		info->args[i] = ft_split(info->av[i + 2], ' ');
		ft_printf("info->args[%i] = ft_split(info->av[%i + 2], ' ')\n", i, i);
		ft_put_strarray(info->args[i]);
		i++;
	}
	info->arg = i;
	return (0);
}

static int	ft_paths(t_pipex_info *info)
{
	int	i;

	ft_printf("Running ft_paths\n");
	i = 0;
	info->paths = NULL;
	//ft_printf("%i = %s\n", i, info->ev[5]);
	while (info->ev[i])
	{
		//ft_printf("%i = %s\n", i, info->ev[i]);
		if (info->ev[i][0] == 'P' && info->ev[i][1] == 'A' && info->ev[i][2] == 'T'
			&& info->ev[i][3] == 'H' && info->ev[i][4] == '=')
		{
			//ft_printf("envp %d = %s\n", i, info->ev[i]);
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

	ft_printf("Running ft_copy_str_array\n");
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
	ft_paths(info);
	ft_args(info);
	i = 0;
	while (info->paths && info->paths[i])
	{
		ft_printf("path %i = %s\n", i, info->paths[i]);
		i++;
	}
	//check here_doc
	ft_printf("opening stuff\n");
	info->fdi = open(info->av[1], O_RDONLY);
	info->fdo = open(info->av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	//check if filedescriptors are okay
	ft_pipes(info);
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
	if (argc > 1)
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
