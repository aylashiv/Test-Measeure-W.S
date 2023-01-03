
#include"Anemometer.h"

struct pollfd fdset;
int nfds = 1;
int fd;
int gpio_fd, timeout, rc;
char *buf[MAX_BUF];
unsigned int gpio;
int len;
char *c;				
int Counter = 0, waitCounter = 3000,actualValue = 0;



/*GPIO EXPORT*/

int gpio_export()
{
	int fd;
	fd = open("/sys/class/gpio/export", O_WRONLY);
	
	if(fd < 0)
	{
		perror("gpio/export");
		return fd;
	}
	write(fd, "20", 3);
	close(fd);

	return 0;
}


/*Set Direction of the GPIO*/
int gpio_set_dir(unsigned int out_Flag)
{
	int fd;      
        fd = open("/sys/class/gpio/gpio20/direction", O_WRONLY);
	if(fd < 0)
	{
           perror("gpio20/direction");
	   return fd;
	}

	if(out_Flag)
	{
		write(fd, "out", 4);
	}
	else
	{
		write(fd, "in", 3);
	}
	close(fd);
	return 0;
}

int gpio_set_edge()
{
	int fd;

	fd = open("/sys/class/gpio/gpio20/edge", O_WRONLY);
	if(fd < 0)
	{
		perror("/gpio20/edge");
		return fd;
	}

	write(fd, "falling", 8);
	close(fd);
	return 0;
}



int WindSpeed(void)
{
	gpio_set_dir(0);
	gpio_set_edge();

	/*Open value file*/
	gpio_fd = open("/sys/class/gpio/gpio20/value", O_RDWR | O_NONBLOCK);
	if(gpio_fd < 0)
	{
		perror("/gpio20/value");
		return gpio_fd;
	}
        
	timeout = 1;

	for(int count = 0; count < 3000; count++)
        {
         memset((void*)&fdset, 0, sizeof(fdset));
	 fdset.fd = gpio_fd;
	 fdset.events = POLLPRI | POLLERR;

	 len = read(fdset.fd, c, 1);
            
	 rc = poll(&fdset, nfds, timeout);

	 if(rc < 0)
	 {
	    printf("\npoll() failed");
	    return -1;
	  }
	  if(rc == 0)
	  {
		//printf(".");
          }
	  if(fdset.revents & POLLPRI)
	  {
		lseek(fdset.fd, 0, SEEK_SET);
		len = read(fdset.fd, buf, MAX_BUF);
                Counter++;
		//printf("%d\n", Counter);
      	   }
	   if(fdset.revents & POLLERR)
	   {
		   //perror("");
		   //printf("errno: %d", errno);
	   }
	  }
	  actualValue = Counter;
	  Counter = 0;
	  close(gpio_fd);
	  return actualValue;
}



