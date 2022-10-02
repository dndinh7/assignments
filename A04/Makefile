CC=gcc
% :: %.c
	$(CC) -g -Wall -Wvla -Werror $< -o $@

test_ppm: test_ppm.c read_ppm.c
	gcc -g -Wall -Wvla -Werror test_ppm.c read_ppm.c -o test_ppm
	
ascii_image: ascii_image.c read_ppm.c
	gcc -g -Wall -Wvla -Werror ascii_image.c read_ppm.c -o ascii_image

all : test_ppm ascii_image

clean :
	rm test_ppm ascii_image 2> /dev/null
