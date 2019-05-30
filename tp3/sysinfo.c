#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define   BUFF_SIZE   1461

int main(void){
	printf("Content-Type: application/json \n\n");
	
	FILE *command;
	char buffer[BUFF_SIZE];
	char command_buffer[BUFF_SIZE];
	int n, tot_mem, used_mem, mem_perc, cpu;
	memset(buffer, '\0', BUFF_SIZE);
	// ------ USED MEMORY -------
	memset(command_buffer, '\0', BUFF_SIZE);
	command = popen("vmstat -s | head -2 | awk '{print $1}'", "r");
	if (command == NULL) {
	printf("Failed to run command\n" );
	exit(1);
	}
	n = fread(command_buffer, 1, BUFF_SIZE, command);
	char * val = strtok(command_buffer, " \n");
	tot_mem = atoi(val);

	char * val2 = strtok(NULL, "\n");
	used_mem = atoi(val2);

	pclose(command);
	mem_perc = (used_mem*100)/tot_mem;

	sprintf(command_buffer, "%d", mem_perc);
		;
	strcat(buffer, "{\n\t\"data\":{\n\t\t\"memory\": ");
	strcat(buffer,command_buffer);
	strcat(buffer,", ");

	// -------- USED CPU ----------
	memset(command_buffer, '\0', BUFF_SIZE);
	command = popen("vmstat 1 2 | tail -1 | awk '{print $15}'", "r");
	if (command == NULL) {
	printf("Failed to run command\n" );
	exit(1);
	}
	n = fread(command_buffer, 1, BUFF_SIZE, command);
	pclose(command);
	command_buffer[n-1]='\0';
	strcat(buffer, "\n\t\t\"cpu\": ");

	cpu = 100-atoi(command_buffer);

	sprintf(command_buffer, "%d", cpu);
	strcat(buffer,command_buffer);
	strcat(buffer,", ");

	memset(command_buffer, '\0', BUFF_SIZE);
	command = popen("uptime -p", "r");
	if (command == NULL) {
	printf("Failed to run command\n" );
	exit(1);
	}
	n = fread(command_buffer, 1, BUFF_SIZE, command);
	pclose(command);
	command_buffer[n-1]='\0';
	strcat(buffer, "\n\t\t\"uptime\": \"");
	strcat(buffer,command_buffer);
	strcat(buffer,"\", ");

	memset(command_buffer, '\0', BUFF_SIZE);
	command = popen("date | awk '{printf(\"%s %s %s %s     %s \", $1, $2, $3, $6, $4)}'", "r");
	if (command == NULL) {
	printf("Failed to run command\n" );
	exit(1);
	}
	n = fread(command_buffer, 1, BUFF_SIZE, command);
	pclose(command);
	command_buffer[n-1]='\0';
	strcat(buffer, "\n\t\t\"datetime\": \"");
	strcat(buffer,command_buffer);
	strcat(buffer,"\"\n\t}\n} ");
	
	printf("%s\n", buffer);
	return 0;
}
