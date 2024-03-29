#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

#define SERVER_PORT 21

typedef struct URL
{
  char ip[128];
  char host[256];
  char path[256];
  char filename[128];
  char user[128];
  char password[128];
} URL;

void printURL(const struct URL *url)
{
  printf("IP: %s\n", url->ip);
  printf("Host: %s\n", url->host);
  printf("Path: %s\n", url->path);
  printf("Filename: %s\n", url->filename);
  printf("User: %s\n", url->user);
  printf("Password: %s\n", url->password);
}

// get ip from host
// given by professor
int getIP(char *host, char *ip)
{
  struct hostent *h;

  if ((h = gethostbyname(host)) == NULL)
  {
    herror("gethostbyname()");
    exit(-1);
  }

  strcpy(ip, inet_ntoa(*((struct in_addr *)h->h_addr)));

  return 0;
}

// establish connection with server
// given by professor
int connectSocket(char *ip, int port)
{
  int sockfd;
  struct sockaddr_in server_addr;

  /*server address handling*/
  bzero((char *)&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip); /*32 bit Internet address network byte ordered*/
  server_addr.sin_port = htons(port);          /*server TCP port must be network byte ordered */

  /*open a TCP socket*/
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket()");
    exit(-1);
  }
  /*connect to the server*/
  if (connect(sockfd,
              (struct sockaddr *)&server_addr,
              sizeof(server_addr)) < 0)
  {
    perror("connect()");
    exit(-1);
  }

  return sockfd;
}

int parse(char *url, struct URL *URL)
{
  printf("URL: %s\n", url);

  char *ftp = strtok(url, "/");          
  char *credentials = strtok(NULL, "/"); 
  char *path = strtok(NULL, "");         

  if (strcmp(ftp, "ftp:") != 0)
  {
    printf("Error: Not using ftp protocol\n");
    return -1;
  }

  char *user = strtok(credentials, ":");
  printf("User: %s\n", user);
  char *password = strtok(NULL, "@");
  if (user != NULL && password != NULL)
  {
    strcpy(URL->host, strtok(NULL, ""));
  }
  else
  {
    user = "anonymous";
    password = "anonymous";
    strcpy(URL->host, credentials);
  }

  strcpy(URL->path, path);
  strcpy(URL->user, user);
  strcpy(URL->password, password);

  getIP(URL->host, URL->ip);

  char *filename = strrchr(path, '/'); // get last occurrence of '/', returns NULL if not found
  if (filename != NULL)
  {
    filename++; // skip the '/', so that the filename string doesn't contain it
  }
  else
  {
    filename = path; // the path doesn't contain any '/', so it's actually the filename
  }

  strcpy(URL->filename, filename);

  printf("\n\n");
  printURL(URL);
  printf("\n\n");
  return 0;
}

int readResponse(int sockfd)
{
  char response[512];
  ssize_t bytes_read = read(sockfd, response, sizeof(response) - 1);
  if (bytes_read < 0)
  {
    perror("Error reading from socket");
    return -1;
  }
  response[bytes_read] = '\0';
  printf("Server response: \n%s\n", response);
  return 0;
}

int download(struct URL *URL)
{
  int sockfd = connectSocket(URL->ip, SERVER_PORT);
  if (sockfd == -1)
  {
    perror("Error creating socket");
    return -1;
  }

  printf("Status: Connected to server\n");
  if (readResponse(sockfd) < 0)
  {
    perror("Error retrieving response from server");
    return -1;
  }

  char user[256];
  sprintf(user, "user %s\n", URL->user);
  if (write(sockfd, user, strlen(user)) < strlen(user))
  {
    perror("Error writing username to socket");
    return -1;
  }
  printf("Status: Sent user\n");
  if (readResponse(sockfd) < 0)
  {
    perror("Error retrieving response from server after sending username");
    return -1;
  }

  char password[256];
  sprintf(password, "pass %s\n", URL->password);
  if (write(sockfd, password, strlen(password)) < strlen(password))
  {
    perror("Error writing username to socket");
    return -1;
  }

  printf("Status: Sent password\n");
  if (readResponse(sockfd) < 0)
  {
    perror("Error retrieving response from server after sending password");
    return -1;
  }

  char pasv[256];
  sprintf(pasv, "pasv\n");
  if (write(sockfd, pasv, strlen(pasv)) < strlen(pasv))
  {
    perror("Error writing pasv command to socket");
    return -1;
  }
  printf("Status: Sent pasv\n");

  char response[256];
  if (read(sockfd, response, 256) < 0){
    perror("Error retrieving server response after sending pasv command");
    return -1;
  }

  printf("Server Response: \n%s\n", response);

  strtok(response, "(");
  char *ip1 = strtok(NULL, ",");
  char *ip2 = strtok(NULL, ",");
  char *ip3 = strtok(NULL, ",");
  char *ip4 = strtok(NULL, ",");
  char *port1 = strtok(NULL, ",");
  char *port2 = strtok(NULL, ")");

  char ip[32];
  sprintf(ip, "%s.%s.%s.%s", ip1, ip2, ip3, ip4);

  int port = atoi(port1) * 256 + atoi(port2);

  printf("IP: %s\n", ip);
  printf("Port: %d\n", port);

  int sockfd2 = connectSocket(ip, port);
  printf("\nStatus: Socket created\n");

  char retr[500];
  sprintf(retr, "retr %s\n", URL->path);
  if (write(sockfd, retr, strlen(retr)) < strlen(retr)){
    perror("Error writing retr command to socket");
    return -1;
  }

  printf("Retr sent\n");

  char buffer[512];
  int file = open(URL->filename, O_WRONLY | O_CREAT, 0666);
  int bytes;
  while ((bytes = read(sockfd2, buffer, 512)) > 0)
  {
    if (write(file, buffer, bytes) < 0)
    {
      perror("Error writing to file");
      return -1;
    }
  }

  printf("Status: Closing connection\n");

  close(file);
  close(sockfd);
  close(sockfd2);

  return 0;
}

int main(int argc, char *argv[])
{

  if (argc != 2)
  {
    printf("Usage: %s ftp://<user>:<password>@<host>/<url>\n", argv[0]);
    exit(-1);
  }

  struct URL URL;

  if (parse(argv[1], &URL) != 0)
  {
    printf("Error parsing URL\n");
    exit(-1);
  }

  if (download(&URL) != 0)
  {
    printf("Error downloading file\n");
    exit(-1);
  }

  return 0;
}