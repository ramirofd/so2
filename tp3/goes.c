#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	printf("Content-Type: application/json \n\n");
	char * line = NULL;
	char * word, * word1;
	int i = 0, lines = 0;
	char buffer[500];
	size_t len = 0;
	ssize_t read;
	FILE * command;
	command = popen("aws s3 ls s3://noaa-goes16/ABI-L2-CMIPF/2017/360 --recursive --no-sign-request --human-readable | grep OR_ABI-L2-CMIPF-M3C13_G16", "r");
	printf("{\n\t\"data\":\n\t\t{");
	while((read = getline(&line, &len, command)) != -1)
	{
		lines++;
	}
	pclose(command);
	command = popen("aws s3 ls s3://noaa-goes16/ABI-L2-CMIPF/2017/360 --recursive --no-sign-request --human-readable | grep OR_ABI-L2-CMIPF-M3C13_G16", "r");
	while((read = getline(&line, &len, command)) != -1)
	{	
		line[strlen(line)-1] = '\0';
		word = strtok(line, " ");
		printf("\"%d\":{\"date\":\"%s\", ", i, word);
		word = strtok(NULL, " ");
		printf("\"time\":\"%s\", ", word);
		word = strtok(NULL, " ");
		word1 = strtok(NULL, " ");
		printf("\"size\":\"%s %s\", ", word, word1);
		word = strtok(NULL, " ");
		printf("\"name\":\"%s\"", word);
		if(i != lines-1)
		{
			printf("},\n\t\t");
		}
		else
		{
			printf("}\n\t\t");
		}
		i++;
	}
	printf("}\n}\n");
	return 0;
}
