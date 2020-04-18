#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFF_LENGTH 255               
static char buff[BUFF_LENGTH];     
 
int main()
{
   	int ret, fd;
   
   	printf("Testing of simple char device driver...\n");
	printf("Press Enter button to open on cdd\n");
   	getchar();
   	fd = open("/dev/cdd", O_RDWR);             
   	if (fd < 0)
	{
      		perror("Failed to open the cdd.");
      		return errno;
   	}
   
	printf("Press Enter button to write on cdd\n");
   	getchar();
   	ret = write(fd,"test", 4); 
   	if (ret < 0)
	{
      		perror("Failed to write on cdd.");
      		return errno;
   	}
 
   	printf("Press Enter button to read cdd\n");
   	getchar();
 
   	ret = read(fd, buff, BUFF_LENGTH);        
   	if (ret < 0)
	{
      		perror("Failed to read the cdd.");
      		return errno;
   	}

	printf("Press Enter button to close cdd\n");
   	getchar();
   
   	printf("End of the program\n");
   	return 0;
}
