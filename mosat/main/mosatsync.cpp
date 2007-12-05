

#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h> 
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MYPORT 3489    /* the port users will be connecting to */

#define BACKLOG 10     /* how many pending connections queue will hold */

void gen_log (char actualhour[],char newhour[])
{
 char date[50];
 time_t t=time(NULL);
 struct tm ttm;
 localtime_r(&t,&ttm);
 strftime(date,sizeof(date),"%d/%m/%Y",&ttm);
 FILE *fp;
 fp=fopen("/nvram/siteur/sync_log.txt","a");
 if(fp!=NULL)
 {
  fprintf(fp,"%s\tActual: %s\tNew: %s\n",date,actualhour,newhour);
  fclose(fp);
 }
 else
  printf("Cannot create file\n");
 
}

int main(int argc, char *argv[])
{
  time_t t;
  struct tm mytm;
  char mybuf[200];
  int sockfd, new_fd;  /* listen on sock_fd, new connection on new_fd */
  struct sockaddr_in my_addr;    /* my address information */
  struct sockaddr_in their_addr; /* connector's address information */
  socklen_t sin_size;

  sockfd = socket(AF_INET, SOCK_STREAM, 0); /* do some error checking! */

  my_addr.sin_family = AF_INET;         /* host byte order */
  my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
  my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
  memset(&(my_addr.sin_zero), 8, 0);        /* zero the rest of the struct */

  /* don't forget your error checking for these calls: */
  bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));

  listen(sockfd, BACKLOG);

  sin_size = sizeof(struct sockaddr_in);
  int child_n;
  for (int i=1; true; i++)
  {
    printf("Proceso padre\n");
    printf("Dormire esperando una coneccion en %d\n", MYPORT);
    //system("ping 172.16.115.28");
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    printf("newfd %i\n", new_fd);
    child_n = i;
    pid_t pid = fork();
    if (pid == 0) break;

    
  }
  char buf[200]; 
  printf("%d: Proceso hijo, en un ciclo infinito para servir al socket\n", child_n);
  while (true)
  {
    //char * mys = "Hurry, send something, I'm waiting!\n";
    //printf("%d: Sending this insult: %s\n", child_n, mys);
    write(new_fd, "Hola", 5); 
    int n = read(new_fd, buf, sizeof(buf) - 1);
    printf("Leyendo mensaje: %s\n", buf);
    if (n < 0)
    {
      perror("Leyendo socket");
      break;
    }
    else if (n > 0)
    {
      buf[n] = '\0';
      printf("%d: Msg: %s\n", child_n, buf);
    t = time(NULL);
    localtime_r (&t,&mytm);
    strftime(mybuf,sizeof(mybuf),"%H:%M:%S",&mytm);
    printf("Hora actual: %s\n",mybuf);
    write(new_fd,mybuf,strlen(mybuf)+1);
      char reply[200];
      snprintf(reply, sizeof(reply), 
		      "%s\nGracias!!\n", buf);
      write(new_fd, reply, strlen(reply) + 1); 
      gen_log(mybuf,buf);
      char systemdate[100];
      sprintf(systemdate,"date -s %s",buf);
      system(systemdate);
      system("hwclock --systohc");
      
//      if (0 == strncasecmp(buf, "quit", 4))
//      {
        break;
//      }
    }
    else
    {
      printf("%d: 0 bytes read, probably an error!\n", child_n);
      break;
    }
    usleep(100000);
  }

//  printf("Help me, I am the kid, I will die . . . . \n");

}
