#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arduino-serial-lib.h"

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	const int decide = 0;
	const int buffer_max = 32;
    const int timeout = 5000;
	const int baudrate = 9600;
    const char eolchar = '\n';
	char serialport[buffer_max];
	int fd = -1;
    char buffer[buffer_max];
    int rc;
	time_t current = time(NULL);
	struct tm *ctm = localtime(&current);
	char hour = ctm->tm_hour;

	if (fd != -1) {
		serialport_close(fd);
		printf("Closed port %s\n", serialport);
	}
	// You may need to change the port depending on
	// your installation.
	// the linux command: dmesg \ grep tty
	// will display the correct port to be used.
	strcpy(serialport, "/dev/ttyACM0");
	fd = serialport_init(serialport, baudrate);
	if (fd < 0)
		error("Error Opening Serial Port\n");
	serialport_flush(fd);	
	printf("opened-serial-port: %s\n", serialport);

	//write the current hour as a byte to arduino
	rc = serialport_writebyte(fd, hour);
	if (rc < 0)
		error("ERROR Writing to Serial Port\n");
	printf("wrote data to serialport: %d\n", hour);

	//read response to display on screen
	rc = serialport_read_until(fd, buffer, eolchar, buffer_max, timeout);
	if( rc < 0)
		error("Error reading from arduino\n");
	printf("received: %s\n", buffer);

	return 0;
}
