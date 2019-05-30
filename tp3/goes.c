#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	printf("Content-Type: application/json \n\n");
	char * line = NULL;
	char buffer[500];
	char response[200000];
	size_t len = 0;
	ssize_t read;
	FILE * command;
	command = popen("aws s3 ls s3://noaa-goes16/ABI-L2-CMIPF/2017/360 --recursive --no-sign-request --human-readable | grep OR_ABI-L2-CMIPF-M3C13_G16", "r");
	printf("{\n\t\"data\":[\n\t\t");
	memset(response, '\0', 200000);
	while((read = getline(&line, &len, command)) != -1)
	{	
		memset(buffer, '\0', 500);
		line[read-1] = '\0';
		strcat(buffer, "{");
		char * text = strtok(line," ");
		strcat(buffer, "\"date\":\"");
		strcat(buffer, text);
		strcat(buffer, "\", ");
		
		text = strtok(NULL, " ");
		strcat(buffer, "\"time\":\"");
		strcat(buffer, text);
		strcat(buffer, "\", ");
		
		text = strtok(NULL, " ");
		strcat(buffer, "\"weight\":\"");
		strcat(buffer, text);
		strcat(buffer, " ");
		text = strtok(NULL, " ");
		strcat(buffer, text);
		strcat(buffer, "\", ");

		text = strtok(NULL, " ");
		text = strtok(text, "/");
		text = strtok(NULL, "/");
		text = strtok(NULL, "/");
		text = strtok(NULL, "/");
		text = strtok(NULL, "/");
		strcat(buffer, "\"name\":\"");
		strcat(buffer, text);
		strcat(buffer, "\"");

		strcat(buffer, "},\n\t\t");
		strcat(response, buffer);
	}
	printf("%s", response);
	printf("]\n}");
	return 0;
}
