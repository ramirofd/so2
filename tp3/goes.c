#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	printf("Content-Type: text/plain\n\n");
	char * line = NULL;
	char buffer[500];
	size_t len = 0;
	ssize_t read;
	FILE * command;
	char command_str[300];
	char * qs = getenv("QUERY_STRING");
	char * year_p = strtok(qs, "&");
	char * doi_p = strtok(NULL,"&");
	year_p = strtok(year_p, "=");
	year_p = strtok(NULL, "=");
	
	doi_p = strtok(doi_p, "=");
	doi_p = strtok(NULL, "=");
	
	memset(command_str, '\0', 300);
	sprintf(command_str, "aws s3 ls s3://noaa-goes16/ABI-L2-CMIPF/%s/%s --recursive --no-sign-request --human-readable | grep OR_ABI-L2-CMIPF-M3C13_G16", year_p, doi_p);
	command = popen(command_str, "r");
	while((read = getline(&line, &len, command)) != -1)
	{	memset(buffer, '\0', 500);
		line[read-1] = '\0';
		strcat(buffer, "<tr>\n");
		char * text = strtok(line," ");
		strcat(buffer, "\t<td>");
		strcat(buffer, text);
		strcat(buffer, "</td>\n");
		
		text = strtok(NULL, " ");
		strcat(buffer, "\t<td>");
		strcat(buffer, text);
		strcat(buffer, "</td>\n");
		
		text = strtok(NULL, " ");
		strcat(buffer, "\t<td>");
		strcat(buffer, text);
		strcat(buffer, " ");
		text = strtok(NULL, " ");
		strcat(buffer, text);
		strcat(buffer, "</td>\n");

		text = strtok(NULL, " ");
		text = strtok(text, "/");
		text = strtok(NULL, "/");
		text = strtok(NULL, "/");
		text = strtok(NULL, "/");
		text = strtok(NULL, "/");
		strcat(buffer, "\t<td>");
		strcat(buffer, text);
		strcat(buffer, "</td>\n");

		strcat(buffer, "</tr>\n");
		printf("%s", buffer);
	}
	return 0;
}
