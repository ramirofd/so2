#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>
#include <fcntl.h> 
#include <errno.h> 
#include <omp.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}

#define INPUT_FILE "OR_ABI-L2-CMIPF-M6C02_G16_s20191011800206_e20191011809514_c20191011809591.nc"

/* Lectura de una matriz de 21696 x 21696 */
#define NX 21696
#define NY 21696
#define KERN_SIZE 3
uint64_t time_stamp_counter()
{
  unsigned int lo, hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t) hi << 32) | lo;
}

void convolute(float *data_in, float *data_out, float kernel[KERN_SIZE][KERN_SIZE], int threads)
{
    int x, y, i, j;
    #pragma omp parallel for shared(data_out) num_threads(threads)
    for ( x=0; x<NX-KERN_SIZE+1; x++ )
        {
            for( y=0; y<NY-KERN_SIZE+1; y++)
                {
                    for(i=0; i<KERN_SIZE; i++)
                        {
                            for(j=0; j<KERN_SIZE; j++)
                                {
                                    *(data_out+((x+1)*NX)+(y+1)) += kernel[i][j] * (*(data_in+((x+i)*NX)+(y+j)));
                                }
                        }
                }
        }
    return;
}

int main(int argc, char *argv[])
{
    int inncid, invarid;
    float *data_in, *data_out;
    int retval, fd, threads, verbose=0;
    uint64_t t1, t2, t3;

    float kernel[3][3] =   {{-1.0, -1.0, -1.0}, 
                            {-1.0,  8.0, -1.0},
                            {-1.0, -1.0, -1.0}};

    if(argc<2)
        {
            printf("Missing Thread number.\n");
            printf("Usage: ./main [NUM_THREADS] [OPTS]\n");
            printf("\ts\t Save to binary file (out.bin).\n");
            printf("\tv\t Verbose mode.\n");
            printf("Example: ./main 4 sv\n");
            printf("This will execute the process with 4 threads for the calculation, ");
            printf("will save the result and also print to console states of the execution.\n");
            return -1;
        }
    else
        {
            threads = atoi(argv[1]);
            if(argc==3)
            {
                if(strstr(argv[2], "v") != NULL)
                    {
                        verbose = 1;
                    }
                else
                    {
                        verbose = 0;
                    }
            }
        }

    

    if(verbose)
        {
            printf("Allocation of memory for input and output arrays.\n");
        }
    data_in = calloc(NX*NY, sizeof(float));
    data_out = (float*)calloc(NX*NY, sizeof(float));
    
    if(verbose)
        {
            printf("Reading NetCDF file.\n");
        }
	if ((retval = nc_open(INPUT_FILE, NC_NOWRITE, &inncid)))
		ERR(retval);
    
	/* Obtenemos elvarID de la variable CMI. */
	if ((retval = nc_inq_varid(inncid, "CMI", &invarid)))
		ERR(retval);

	/* Leemos la matriz. */
	if ((retval = nc_get_var_float(inncid, invarid, data_in)))
  	    ERR(retval);

     /* Se cierra el archivo y liberan los recursos*/
	if ((retval = nc_close(inncid)))
		ERR(retval); 
    
    if(verbose)
        {
            printf("Starting process with: %d threads.\n", threads);
        }
	t1 = time_stamp_counter();
    t2 = time_stamp_counter();

    convolute(data_in, data_out, kernel, threads);

    t3 = time_stamp_counter();

    if(verbose)
        printf("Filter operation cycles: ");

    printf("%15lu\n", ((t3-t2)-(t2-t1)));

    if(argc >= 3)
        {
            
            if(strstr(argv[2], "s") != NULL)
                {
                    if(verbose)
                        {
                            printf("Writing output to binary file \"out.bin\".\n");
                            printf("\t Note: to get an image execute to_img.py.\n");
                        }
                    if((fd = open("out.bin", O_WRONLY|O_CREAT|O_TRUNC, 0666))<0)
                        {
                            printf("Error abriendo archivo\n");
                            return -1;
                        }
                    if(write(fd, data_out, (NX)*(NY)*sizeof(float))<0)
                        {
                            perror("ERROR escribiendo archivo");
                            return -1;
                        }
                    close(fd);
                }
        }

    if(verbose)
        {
            printf("Freeing input and output arrays memory.\n");
        }
    free(data_in);
    free(data_out);
	return 0;
}
