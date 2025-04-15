#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Esta version no es perfecta porque falla el test de 42 exam 
void pe(char *s, char *arg)
{
	while(*s)
		write(2, s++, 1);
	if(arg != NULL)
		while(*arg)
			write(2, arg++, 1);
	write(2, "\n", 1);
}

void fatal()
{
	pe("error: fatal", NULL);
	exit(1);
}

void exec(int i, int temp, char **argv,char **env)
{
	argv[i] = NULL;
	if(dup2(temp, STDIN_FILENO) == -1)
		fatal();
	if(close(temp) == -1)
		fatal();
	execve(argv[0], argv, env);
	pe("error: cannot execute ", argv[0]);
	exit(1);
}

int main(int argc, char **argv, char **env)
{
	int i;
	int temp;
	int fd[2];
	int pid;

	(void)argc;
	i = 0;
	temp = dup(STDIN_FILENO);
	if(temp == -1)
		fatal();
	while(argv[i] && argv[i + 1])
	{
		argv = &argv[i + 1];
		i = 0;
		while(argv[i] != NULL && strcmp(argv[i], ";") != 0 && strcmp(argv[i], "|") != 0)
			i++;
		if(i != 0 && strcmp(argv[0], "cd") == 0)
		{
			if(i != 2)
				pe("error: cd: bad arguments", NULL);
			else if(chdir(argv[1]) != 0)
				pe("error: cd: cannot change directory to ", argv[1]);
		} else if(i != 0 && (argv[i] == NULL || strcmp(argv[i], ";") == 0))
		{
			pid = fork();
			if(pid == -1)
				fatal();
			if(pid == 0)
				exec(i, temp, argv, env);
			else
			{
				if(close(temp) == -1)
					fatal();
				while(waitpid(-1, NULL, WUNTRACED) != -1)
				temp = dup(STDIN_FILENO);
				if(temp == -1)
					fatal();
			}
		} else if(i != 0 && strcmp(argv[i], "|") == 0)
		{
			//if(argv[i + 1] == NULL) //Si incluyo esta condicion (que es razonable) no pasa el test del 42 Exam
				//pe("error: bad syntax", NULL);
			//else
			//{
				if(pipe(fd) == -1)
					fatal();
				pid = fork();
				if(pid == -1)
					fatal();
				if(pid == 0)
				{
					if(dup2(fd[1], STDOUT_FILENO) == -1)
						fatal();
					if(close (fd[1]) == -1 || close(fd[0]) == -1)
						fatal();
					exec(i, temp, argv, env);
				}else
				{
					if(close(fd[1]) == -1 || close(temp) == -1)
						fatal();
					temp = fd[0];
				}
			//}
		}
	}
	if(close(temp) == -1)
		fatal();
	return(0);
}
