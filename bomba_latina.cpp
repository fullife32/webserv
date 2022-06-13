#include <iostream>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include <string.h>
#include <unistd.h>
int main() {

	char s[1024 * 10];
	memset(&s, 'V', 1024 * 10);
	int fd = open("hole.txt", O_WRONLY | O_CREAT | O_APPEND);
	for (int i = 0; i < 1000; i++) {
		write(fd, s, 1024 * 10);
		write(fd, "\n", 1);
	}
	// write(fd, "\0", 1);
}
