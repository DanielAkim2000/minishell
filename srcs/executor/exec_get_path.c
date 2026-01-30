/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demane <emanedanielakim@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:18:55 by demane            #+#    #+#             */
/*   Updated: 2026/01/30 17:29:23 by demane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

int	ft_get_index_path(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (i);
	}
	return (-1);
}

int	ft_access(char **path_params)
{
	int		acs_val;
	char	*path;

	path = *path_params;
	if (!path)
		return (0);
	if (ft_strchr(path, '/') == NULL)
		return (0);
	acs_val = access(path, X_OK);
	if (acs_val == 0)
		return (1);
	return (0);
}

int	ft_access_in_rep(char **path_params, char ***split_val,
	t_shell *shell)
{
	int		acs_val;
	char	*path;
	char	*check;

	path = *path_params;
	if (!path)
		return (0);
	check = ft_strchr(path, '/');
	if (check == NULL)
		return (0);
	acs_val = access(path, X_OK);
	if (acs_val == 0)
		return (1);
	if (errno == EACCES)
	{
		ft_perror(path);
		ft_free_split(split_val);
		ft_free_shell(&shell);
		exit(126);
	}
	return (0);
}

char	*ft_try_paths(char ***split_val, char *cmd_name, t_shell *shell)
{
	char	*path;
	int		i;

	i = -1;
	path = NULL;
	while ((*split_val)[++i])
	{
		path = ft_strjoin_sep((*split_val)[i], cmd_name, '/');
		if (!path)
		{
			ft_free_split(split_val);
			ft_free_shell(&shell);
			exit(1);
		}
		if (ft_access(&path))
			return (ft_free_split(split_val), path);
		ft_free((void **)&path);
	}
	return (ft_free_split(split_val), NULL);
}

char	*ft_get_path(t_ast *node, t_shell *shell)
{
	char	*path;
	char	**split_val;
	t_vars	*var;
	char	*cmd;

	path = NULL;
	cmd = node->args[0];
	if (!cmd)
		return (NULL);
	if (ft_strlen(cmd) == 0 || ft_str_is_space(cmd))
		return (NULL);
	if (ft_access_in_rep(&(cmd), NULL, shell)
		|| (ft_is_directory(cmd) == TRUE && ft_strchr(cmd, '/')))
		return (ft_strdup(cmd));
	var = ft_find_vars("PATH", shell);
	if (!var || var->value == NULL || !*var->value)
		return (NULL);
	split_val = ft_split(var->value, ':');
	if (!split_val)
		return (NULL);
	path = ft_try_paths(&split_val, cmd, shell);
	return (path);
}
