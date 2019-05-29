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
	int n;
	memset(buffer, '\0', BUFF_SIZE);

	memset(command_buffer, '\0', BUFF_SIZE);
	command = popen("free -m | grep Mem | awk '{printf(\"%2.1f\", (($2-$4)/$2)*100.0)}'", "r");
	if (command == NULL) {
	printf("Failed to run command\n" );
	exit(1);
	}
	n = fread(command_buffer, 1, BUFF_SIZE, command);
	pclose(command);
	strcat(buffer, "{\n\t\"data\":{\n\t\t\"memory\": ");
	strcat(buffer,command_buffer);
	strcat(buffer,", ");

	memset(command_buffer, '\0', BUFF_SIZE);
	command = popen("ps -A -o pcpu | tail -n+2 | paste -sd+ | bc", "r");
	if (command == NULL) {
	printf("Failed to run command\n" );
	exit(1);
	}
	n = fread(command_buffer, 1, BUFF_SIZE, command);
	pclose(command);
	command_buffer[n-1]='\0';
	strcat(buffer, "\n\t\t\"cpu\": ");
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
	command = popen("hostname", "r");
	if (command == NULL) {
	printf("Failed to run command\n" );
	exit(1);
	}
	n = fread(command_buffer, 1, BUFF_SIZE, command);
	pclose(command);
	command_buffer[n-1]='\0';
	strcat(buffer, "\n\t\t\"hostname\": \"");
	strcat(buffer,command_buffer);
	strcat(buffer,"\"\n\t}\n} ");
	
	printf("%s\n", buffer);
	return 0;
}
