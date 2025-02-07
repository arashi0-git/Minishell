


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
