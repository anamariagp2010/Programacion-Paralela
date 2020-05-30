COMPILE SEQUENTIAL IMPLEMENTATION (LINUX):

gcc -o (executable file name) bmp.c image.c image.h main.c

COMPILE PARALLEL IMPLEMENTATION (LINUX):

gcc -o (executable file name) bmp.c image.c image.h main.c -lpthread


EXECUTE SEQUENTIAL IMPLEMENTATION (LINUX):

./(Executable_file_name) Input_file_name.bmp Output_file_name.bmp (Toolbox_function_number)

EXECUTE PARALLEL IMPLEMENTATION (LINUX):

./(Executable_file_name) Input_file_name.bmp Output_file_name.bmp (Number_of_threads_of_execution) (Toolbox_function_number)


TOOLBOX FUNCTIONS NUMBERS:

1) Curvecurve: This distorts the image.

2) Low: This divides the height of the image in half.

3) Fat: This doubles the width of the image.

4) High: This doubles the height of the image.

5) Thin: This halves the width of the image.

6) Opposite: This rotates the image 180 degrees.