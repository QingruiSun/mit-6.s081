#include "kernel/types.h"
#include "user/user.h"

#define R 0
#define W 1


void redirect(int k, int pd[]) 
{
	close(k);
	dup(pd[k]);
	close(pd[R]);
	close(pd[W]);
}

void filterNumbers(int n)
{
	int p;
	while (read(R, &p, sizeof(p)))
	{
		if (p % n != 0)
		{
			write(W, &p, sizeof(p));
		}

	}
	close(W);
	wait(0);
	exit(0);
}

void waitForNumbers() 
{
	int pd[2];
	int p;
	if (read(R, &p, sizeof(p)))
	{
		pipe(pd);
		printf("prime %d\n", p);
		if (fork() == 0) // child process.
		{
			redirect(R, pd);
			waitForNumbers();
			
		}
		else            //parent process.
		{
			redirect(W, pd);
			filterNumbers(p);
		}
	}
	exit(0);
}




int main(int argc, char *argv[])
{
	int pd[2];
	pipe(pd);
        if (fork() == 0) {   //child process.
		redirect(R, pd);
		waitForNumbers();

	} else {             // parent process.
		redirect(W, pd);
		for (int i = 2; i <= 35; ++i) {
			write(W, &i, sizeof(i));
		}
		close(W);
		wait(0);
		exit(0);
		
	}
	return 0;

}

