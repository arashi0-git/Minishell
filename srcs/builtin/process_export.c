static bool is_valid_identifier(const char *str)
{
    char *equal_pos;
    size_t  key_length;
    size_t  i;
    
    equal_pos = NULL;
    i = 0;
    if (!str || !*str)
        return (false);
    if (!ft_is_alpha(*str) || *str != '_')
        return (false);
    equal_pos = ft_strchr(str, '=');
    if (equal_pos)
        key_length = equal_pos - s;
    else 
        key_length = ft_strlen(str);
    i = 1;
    while (i < key_length)
    {
        if (!ft_is_alnum(str[i]) && str[i] != '_')
            return (false);
        i++;
    }
    return (true);
}

int	exec_export(char **args, t_shell *shell)
{
	int i;

    if (!args[1])
		//return (print_sorted_env(shell->env));
    i = 1;
    while (args[i])
    {
        if (is_valid_identifier(args[i]))
            //update_env_variable(shell->env, args[i]);
        i++;
    }
    return (0);
}
/*
   export [-fn] [name[=word]] ...
       export -p
              The  supplied names are marked for automatic export to the environment of subse‐
              quently executed commands.  If the -f option is given, the names refer to  func‐
              tions.   If no names are given, or if the -p option is supplied, a list of names
              of all exported variables is printed.  The -n option causes the export  property
              to  be  removed  from  each  name.  If a variable name is followed by =word, the
              value of the variable is set to word.  export returns an exit status of 0 unless
              an invalid option is encountered, one of the names is not a valid shell variable
              name, or -f is supplied with a name that is not a function.

	
	#example
	 export MY_VAR="hello"
	# この後に実行される全てのコマンドで
	# MY_VARという環境変数が利用可能になる

	bash  # 新しいシェルを起動
	echo $MY_VAR  # "hello"が表示される
	→子プロセスに継承されるため
*/
