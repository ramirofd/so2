#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	FILE *out;
	char *rawdata; // pointer for rawdata
	char *data; // will be an offset of rawdata after the two newlines
	unsigned long length; // length of data to be read used for malloc
	unsigned long writelength; // use for the length of the write
	char *pos; // used in the loop

	printf("Content-type: text/html\n\n<HTML><PRE>"); // for debug output
	// Various bits of debug output are included - comment out for live
	// and replace with whatever output you'd like to send to the client

	out=fopen("/var/www/test.jpg","wb+"); // open the output file

	length=atol(getenv("CONTENT_LENGTH"));
	writelength=length;

	//printf("Content Length: %u\n",length);

	rawdata=malloc(length+1);

	fread(rawdata, length, 1, stdin);

	for (pos=rawdata; pos<(rawdata+length-4); pos++)
	{
		 writelength--;
		 //printf("%c %d\n",pos[0],pos[0]);
		 if ( (pos[0]==13) && (pos[1]==10) && (pos[2]==13) && (pos[3]==10) && ( (pos[4]<32)||(pos[4]>127) ) )
		 {
		 	data=pos+4;
			pos=rawdata+length+2;
			//printf("Found\n");
			writelength-=3;
		 }
	}

	//printf("Writelength: %u\n",writelength);

	fwrite(data, 1, writelength, out);

	fclose(out);

	free(rawdata);

	//printf("Upload Complete");

	return 0;
}
