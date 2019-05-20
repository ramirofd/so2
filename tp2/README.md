# TP2
Trabajo Practicos de Sistemas Operativos 2 - UNC - FCEFyN

Edge Filter Algorithm with OpenMP optimization.

# Build and run
### Local
* run *make local*
* run *./main \[NUM_THREADS\] \[OPTS\]* in a terminal.

### Cluster
* run *make cluster*
* run **(Only once)** *export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:\<path to netcdf library\>*
* run *./main \[NUM_THREADS\] \[OPTS\]* in a terminal.

#Clean
* run *make clean*

# Options
* **v** Verbose mode.
* **s** Save result to file (out.bin).

# Example
~~~
$ ./main 4 vs
~~~
This will execute the process with 4 threads for the filtered image calculation, will save the result and also print to console states of the execution.

Should show something like:
~~~
Allocation of memory for input and output arrays.
Reading NetCDF file.
Starting process with: 4 threads.
Filter operation cycles:      6472083207
Writing output to binary file "out.bin".
	 Note: to get an image execute to_img.py.
Freeing input and output arrays memory.
~~~

# Binary/NetCDF to image.jpg

To get a visible image in JPG format use the Python script named to_img.py. ***Requires Numpy and OpenCV***.

Usage:
~~~
$ python3 to_image.py
~~~



