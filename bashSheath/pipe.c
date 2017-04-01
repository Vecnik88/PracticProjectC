// Created by Vecnik88 01.04.2017

/* 
	Реализовано некое подобие оболочки bash с возможностью вызова программ друг за другом в виде:

		ls | who | sort

	Реализована возможность ввода от 2 до 4 параметров.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_BUFFER 200
#define SYMBOL "|\n"
#define SYMBOL_SPACE " "

void pipe2arg(char** str1, char** str2);
void pipe3arg(char** str1, char** str2, char** str3);
void pipe4arg(char** str1, char** str2, char** str3, char** str4);

void strCommand(char** buffer, char** str);

int main()
{
	char** str1 = (char**) malloc(sizeof(char*)*5);
	char** str2 = (char**) malloc(sizeof(char*)*5);
	char** str3 = (char**) malloc(sizeof(char*)*5);
	char** str4 = (char**) malloc(sizeof(char*)*5);
	char buffer[SIZE_BUFFER];
	char** words = NULL;
	char* p = NULL;

	int count = 0;
	int status;

	pid_t child;

	printf("Enter your code this: ");
	fgets(buffer, SIZE_BUFFER - 1, stdin);
	
	for (p = strtok(buffer, SYMBOL); p != NULL; p = strtok(NULL, SYMBOL) )
    {
        if ((words = (char**)realloc(words, sizeof(char*) * (count + 1))) == NULL )
        {
            printf("No memory to new word!\n");
            exit(1);
        }
        if ((words[count] = strdup(p)) == NULL )
        {
            printf("Can't duplicate word!\n");
            exit(1);
        }
        ++count;
    }

	switch(count)
	{
		case 2:
		strCommand(words[0], str1);
		strCommand(words[1], str2);
		free(words);
		if((child = fork())==0)
		{
			pipe2arg(str1, str2);
		}
		else
		{
			waitpid(child, &status, NULL);
			sleep(4);
			free(str1); free(str2); free(str3); free(str4);
			printf("END programms. Bye\n");
			return 0;
		}
	
		case 3:
		strCommand(words[0], str1);
		strCommand(words[1], str2);
		strCommand(words[2], str3);
		free(words);
		if((child = fork())==0)
		{
			pipe3arg(str1, str2, str3);
		}
		else
		{
			waitpid(child, &status, NULL);
			sleep(4);
			free(str1); free(str2); free(str3); free(str4);
			printf("END programms. Bye\n");
			return 0;
		}

		case 4:
		strCommand(words[0], str1);
		strCommand(words[1], str2);
		strCommand(words[2], str3);
		strCommand(words[3], str4);
		free(words);
		if((child = fork())==0)
		{
			pipe4arg(str1, str2, str3, str4);
		}
		else
		{
			waitpid(child, &status, NULL);
			sleep(4);
			free(str1); free(str2); free(str3); free(str4);
			printf("END programms. Bye\n");
			return 0;
		}	
	}		
}

void strCommand(char** buffer, char** str)
{
	char* p = NULL;
	int count = 0;
	for (p = strtok(buffer, SYMBOL_SPACE); p != NULL; p = strtok(NULL, SYMBOL_SPACE))
    {
        if ((str[count] = strdup(p)) == NULL )
        {
            printf("Can't duplicate word!\n");
            exit(1);
        }
        ++count;
    }	
}
    
void pipe2arg(char** str1, char** str2)
{
	int fd[2];
	pipe(fd);
	if(fork())
	{
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);

		execvp(str1[0], str1);
	}
	else
	{
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);

		execvp(str2[0], str2);
	}
}

void pipe3arg(char** str1, char** str2, char** str3)
{
	const int index = 4;
	int i = 0;
	int fd[index];					
	pipe(fd);
	pipe(fd + 2);						// <---. создаем 4 дескриптора - 2 read, 2 write
	
	if(fork() == 0)
	{
		dup2(fd[1], 1);

		for(i = 0; i < index; ++i)
			close(fd[i]);
		
		execvp(str1[0], str1);
	}
	else
	{
		if(fork() == 0)
		{
			dup2(fd[3], 1);
			dup2(fd[0], 0);
			
			for(i = 0; i < index; ++i)
				close(fd[i]);

			execvp(str2[0], str2);

		}
		else
		{
			dup2(fd[2], 0);

			for(i = 0; i < index; ++i)
				close(fd[i]);

			execvp(str3[0], str3);
		}
	}
}

void pipe4arg(char** str1, char** str2, char** str3, char** str4)
{
	const int index = 6;
	int i = 0;
	int fd[index];					
	pipe(fd);
	pipe(fd + 2);
	pipe(fd + 4);						// <---. создаем 6 дескрипторов - 3 read, 3 write
	
	if(fork() == 0)
	{
		dup2(fd[1], 1);

		for(i = 0; i < index; ++i)
			close(fd[i]);

		execvp(str1[0], str1);
	}
	else
	{
		if(fork() == 0)
		{
			dup2(fd[3], 1);
			dup2(fd[0], 0);

			for(i = 0; i < index; ++i)
				close(fd[i]);

			execvp(str2[0], str2);

		}
		else
		{
			if(fork() == 0)
			{
				dup2(fd[5], 1);
				dup2(fd[2], 0);

				for(i = 0; i < index; ++i)
					close(fd[i]);

				execvp(str3[0], str3);
			}
			else
			{
				dup2(fd[4], 0);

				for(i = 0; i < index; ++i)
					close(fd[i]);

				execvp(str4[0], str4);
			}
		}
	}
}
