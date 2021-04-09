#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXN 1024

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(2, "usage: xargs command\n");
		exit(1);
	}

	char *_argv[MAXARG];
	for (int i = 1; i < argc; ++i)
	{
		_argv[i - 1] = argv[i];
	}

	char buf[MAXN];
	char c = 0;
	int stat = 1;

	while (stat)
	{
		int _argv_cnt = argc - 1;
		int buf_cnt = 0;
		int arg_begin = 0;

		while (1) // start process one line.
		{
			stat = read(0, &c, 1);
			if (stat == 0)
				exit(0);
			if (c == ' ') // find an argument until find a space.
			{
				buf[buf_cnt++] = 0;
				_argv[_argv_cnt++] = &buf[arg_begin];
				arg_begin = buf_cnt;
			}
			else if (c == '\n') // read a complete line.
			{
				buf[buf_cnt++] = 0;
				_argv[_argv_cnt++] = &buf[arg_begin];
				arg_begin = buf_cnt;
				break;
			}
			else
			{
				buf[buf_cnt++] = c;
			}
		}

		_argv[_argv_cnt] = 0;
		if (fork() == 0)
		{
			exec(_argv[0], _argv);
		}
		else
		{
			wait(0);
		}

	}
	exit(0);
}
