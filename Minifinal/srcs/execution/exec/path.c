#include "minishell.h"

char	*join_path(const char *dir, const char *file)
{
	char	*tmp;
	char	*out;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	out = ft_strjoin(tmp, file);
	free(tmp);
	return (out);
}

static char	*find_cmd_in_paths(char **paths, char *name)
{
	char	*candidate;
	int		i;

	i = 0;
	while (paths[i])
	{
		candidate = join_path(paths[i], name);
		if (candidate && access(candidate, X_OK) == 0)
		{
			return (candidate);
		}
		free(candidate);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *name, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (ft_strchr(name, '/'))
		return (ft_strdup(name));
	path_env = get_env(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = find_cmd_in_paths(paths, name);
	ft_split_free(paths);
	return (result);
}
