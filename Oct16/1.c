#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define RD_END 0
#define WR_END 1

int main(int argc, char const *argv[])
{
	int pipefd1[2], pipefd2[2];
	int status1, status2;

	status1 = pipe(pipefd1);
	if (status1 == -1)
	{
		printf("Pipe Failed\n");
		return 0;
	}

	status2 = pipe(pipefd2);
	if (status2 == -1)
	{
		printf("Pipe Failed\n");
		return 0;
	}
	pid_t pid = fork();
	if (pid > 0)
	{
		//parent
		close(pipefd1[RD_END]);
		close(pipefd2[WR_END]);

		char str1[11] = "Hello World";
		char str2[11];
		memset(str2, 0, sizeof(str2));
		printf("The original string by the parent is '%s'\n", str1);
		write(pipefd1[WR_END], str1, sizeof(str1) + 1);
		read(pipefd2[RD_END], str2, sizeof(str2));
		printf("The reversed string by the child is '%s'\n", str2);
	}
	else
	{
		//child
		close(pipefd1[WR_END]);
		close(pipefd2[RD_END]);

		char str2[11];
		char str3[11];
		memset(str2, 0, sizeof(str2));
		memset(str3, 0, sizeof(str3));
		read(pipefd1[RD_END], str2, sizeof(str2));
		int j = 0;
		int i = sizeof(str2) / sizeof(str2[0]) - 1;
		for (; i >= 0; i--)
		{
			str3[j] = str2[i];
			j++;
		}

		write(pipefd2[WR_END], str3, sizeof(str3) + 1);
	}
	return 0;
}