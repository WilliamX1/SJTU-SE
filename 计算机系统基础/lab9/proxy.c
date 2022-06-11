/*
 * proxy.c - ICS Web proxy
 * id: 519021910861
 * name: huidong xu
 */

#include "csapp.h"
#include <stdarg.h>
#include <sys/select.h>

/* In debug mode, debug information such as printf will be not execute */
sem_t log_mutex;

/*
 * Function prototypes
 */
int parse_uri(char *uri, char *target_addr, char *path, char *port);
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, size_t size);
void doit(int connfd, struct sockaddr_in *clientaddr);
int proxy_send(int clientfd, rio_t *conn_rio, char *requestheader, size_t content_length, char *method, char *pathname, char *version);
size_t proxy_receive(int connfd, rio_t *client_rio);

typedef struct vargp{
    int connfd;
    struct sockaddr_in clientaddr;
} vargp_t;

void *thread(void *vargp){
    Pthread_detach(Pthread_self());
    vargp_t *vargp_self = (vargp_t *)vargp;
    doit(vargp_self->connfd, &(vargp_self->clientaddr));
    Close(vargp_self->connfd);
    Free(vargp_self);
    return NULL;
}

/*
 * main - Main routine for the proxy program
 */
int main(int argc, char **argv)
{
    int listenfd;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    char client_hostname[MAXLINE], client_port[MAXLINE];
    pthread_t tid;
    vargp_t *vargp;

    /* Check arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        exit(0);
    }

    Sem_init(&log_mutex, 0, 1);
    Signal(SIGPIPE, SIG_IGN);
    /* Listen to Client */
    listenfd = Open_listenfd(argv[1]);
    while(1){
        vargp = Malloc(sizeof(vargp_t));
        vargp->connfd = Accept(listenfd, (SA *)&(vargp->clientaddr), &clientlen);
        Pthread_create(&tid, NULL, thread, vargp);
        Getnameinfo((SA *)&(vargp->clientaddr), clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
    }
    /* Remember to free source */
    Close(listenfd);
    exit(0);
}

void doit(int connfd, struct sockaddr_in *clientaddr){
    char buf[MAXLINE], requestheader[MAXLINE];
    char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE], pathname[MAXLINE], port[MAXLINE];
    int clientfd;
    rio_t conn_rio, client_rio;
    size_t n, byte_size = 0, content_length = 0;

    /* Connect to Server */
    Rio_readinitb(&conn_rio, connfd);
    if(Rio_readlineb(&conn_rio, buf, MAXLINE) == 0){
        app_error("error: read empty request line");
        return;
    }
    if(sscanf(buf, "%s %s %s", method, uri, version) != 3){
        app_error("error: mismatched parameters");
        return;
    }
    if(parse_uri(uri, hostname, pathname, port) != 0 ){
        app_error("error: parse uri wrong");
        return;
    }

    /* Request Header */
    sprintf(requestheader, "%s /%s %s\r\n", method, pathname, version);
    while ((n = Rio_readlineb(&conn_rio, buf, MAXLINE)) != 0)
    {
        if(!strncasecmp(buf, "Content-Length", 14))
            sscanf(buf+15, "%zu", &content_length);
        sprintf(requestheader, "%s%s", requestheader, buf);
        if(!strncmp(buf, "\r\n", 2)) break;  
    }
    if(!n) return;

    /* Open Clientfd */
    while ((clientfd = open_clientfd(hostname, port)) < 0){
        app_error("error: open client fd wrong");
        fprintf(stderr, "error: hostname-%s\n", hostname);
        fprintf(stderr, "error: port-%s\n", port);
        return;
    }
    Rio_readinitb(&client_rio, clientfd);

    //Send By Proxy From Client to Server
    if(proxy_send(clientfd, &conn_rio, requestheader, content_length, method, pathname, version) != -1)
        //Send By Proxy From Server to Client
        byte_size = proxy_receive(connfd, &client_rio);

    /* Format Output */
    format_log_entry(buf, clientaddr, uri, byte_size);
    P(&log_mutex);
    printf("%s\n", buf);
    V(&log_mutex);

    Close(clientfd);
}

int proxy_send(int clientfd, rio_t *conn_rio, char *requestheader, size_t content_length, char *method, char *pathname, char *version)
{
    char buf[MAXLINE];

    Rio_writen(clientfd, requestheader, strlen(requestheader));
    /* Request Body */
    if(strcasecmp(method, "GET"))
        for(int i = 0; i < content_length; i++){
            if(Rio_readnb(conn_rio, buf, 1) == 0)
                return -1;
            Rio_writen(clientfd, buf, 1);
        }
    return 0;
}

size_t proxy_receive(int connfd, rio_t *client_rio)
{
    char buf[MAXLINE];
    size_t n, byte_size = 0, content_length = 0;

    /* Response Header */

    while((n = Rio_readlineb(client_rio, buf, MAXLINE)) != 0){
        byte_size += n;
        if(!strncasecmp(buf, "Content-Length: ", 14))
            sscanf(buf+15, "%zu", &content_length);
        Rio_writen(connfd, buf, strlen(buf));
        if(!strncmp(buf, "\r\n", 2)) break;
    }
    if(!n) return 0;

    /* Response Body */
    for(int i = 0; i < content_length; i++){
        if(Rio_readnb(client_rio, buf, 1) == 0) return 0;
        byte_size ++;
        Rio_writen(connfd, buf, 1);
    }
    return byte_size;
}

/*
 * parse_uri - URI parser
 *
 * Given a URI from an HTTP proxy GET request (i.e., a URL), extract
 * the host name, path name, and port.  The memory for hostname and
 * pathname must already be allocated and should be at least MAXLINE
 * bytes. Return -1 if there are any problems.
 */
int parse_uri(char *uri, char *hostname, char *pathname, char *port)
{
    char *hostbegin;
    char *hostend;
    char *pathbegin;
    int len;

    if (strncasecmp(uri, "http://", 7) != 0) {
        hostname[0] = '\0';
        return -1;
    }

    /* Extract the host name */
    hostbegin = uri + 7;
    hostend = strpbrk(hostbegin, " :/\r\n\0");
    if (hostend == NULL)
        return -1;
    len = hostend - hostbegin;
    strncpy(hostname, hostbegin, len);
    hostname[len] = '\0';

    /* Extract the port number */
    if (*hostend == ':') {
        char *p = hostend + 1;
        while (isdigit(*p))
            *port++ = *p++;
        *port = '\0';
    } else {
        strcpy(port, "80");
    }

    /* Extract the path */
    pathbegin = strchr(hostbegin, '/');
    if (pathbegin == NULL) {
        pathname[0] = '\0';
    }
    else {
        pathbegin++;
        strcpy(pathname, pathbegin);
    }

    return 0;
}

/*
 * format_log_entry - Create a formatted log entry in logstring.
 *
 * The inputs are the socket address of the requesting client
 * (sockaddr), the URI from the request (uri), the number of bytes
 * from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr,
                      char *uri, size_t size)
{
    time_t now;
    char time_str[MAXLINE];
    unsigned long host;
    unsigned char a, b, c, d;

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    /*
     * Convert the IP address in network byte order to dotted decimal
     * form. Note that we could have used inet_ntoa, but chose not to
     * because inet_ntoa is a Class 3 thread unsafe function that
     * returns a pointer to a static variable (Ch 12, CS:APP).
     */
    host = ntohl(sockaddr->sin_addr.s_addr);
    a = host >> 24;
    b = (host >> 16) & 0xff;
    c = (host >> 8) & 0xff;
    d = host & 0xff;

    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %d.%d.%d.%d %s %zu", time_str, a, b, c, d, uri, size);
}