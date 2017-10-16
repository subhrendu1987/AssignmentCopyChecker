#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define CLIENT_ID_LEN 20
#define BUF_SIZE 1024
#define MAX_RES_SIZE 65536
#define MAX_REQ_SIZE 65536
#define MAX_CMD_SIZE 32
#define MAX_FIELD_SIZE 32

static struct sockaddr_in server;
const char *end_del = "**********";
static char client_id[CLIENT_ID_LEN];
static char client_password[CLIENT_ID_LEN];

void sockaddr_init(const char *ip, int port);
int authenticate();
char *seek_res(char *res);
void add_field(char *req, const char *field);
void add_auth_fields(char *req);
void end_request(char *req);
void request(const char *req, char *res);
int is_success(const char *res);
void display_help();
int trade_request(int type);
void view_request(int type);


int main(int argc, char const *argv[])
{
    char choice;

    if (argc!=3)
    {
        printf("Usage:\n");
        printf("./client <server IP> <port>\n");
        printf("---------------\n");
        printf("Connect to 127.0.0.1:1234? (Y/n) : ");
        scanf("%c",&choice);
        if(choice!='\n' && choice!='Y' && choice!='y')
            exit(0);
        sockaddr_init("127.0.0.1", 1234);
    }
    else
    {
        sockaddr_init(argv[1], atoi(argv[2]));
    }

    printf("Connected to server\n");
    printf("--------------------\n\n");

    while(1)
    {
        char res[MAX_RES_SIZE];
        printf("Trader id : ");
        scanf("%s", client_id);
        printf("Password : ");
        scanf("%s", client_password);
        if(authenticate(res))
        {
            printf("%s\n", seek_res(res));
            break;
        }
        else
        {
            printf("%s\n", seek_res(res));
        }
    }
    printf("--------------------\n");
    display_help();
    while(1)
    {
        char cmd[MAX_CMD_SIZE];
        printf(">>> ");
        scanf("%s", cmd);
        if(strcmp(cmd, "") == 0)
            continue;
        else if(strcmp(cmd, "buy") == 0)
            trade_request(0);
        else if(strcmp(cmd, "sell") == 0)
            trade_request(1);
        else if(strcmp(cmd, "orders") == 0)
            view_request(0);
        else if(strcmp(cmd, "trades") == 0)
            view_request(1);
        else if(strcmp(cmd, "help") == 0)
            display_help();
        else if(strcmp(cmd, "exit") == 0)
            break;
        else
            printf("Invalid command\n");
    }

    return 0;
}


void sockaddr_init(const char *ip, int port)
{
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
}


char *seek_res(char *res)
{
    char *p = res;
    while(*p != '\n')
        p++;
    p++;
    return p;
}


void add_field(char *req, const char *field)
{
    strcat(req, field);
    strcat(req, "\n");
}


void add_auth_fields(char *req)
{
    add_field(req, client_id);
    add_field(req, client_password);
}


void end_request(char *req)
{
    strcat(req, end_del);
}


void request(const char *req, char *res)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        printf("ERROR : Unable to create socket\n");
        exit(-1);
    }
    if( connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0 )
    {
        printf("ERROR : Unable to connect to server\n");
        exit(-1);
    }
    if( send(sock, req, strlen(req), 0) < 0 )
    {
        printf("ERROR : Unable to send to server\n");
        exit(-1);
    }
    char *buf[BUF_SIZE];
    int offset = 0;
    while(1)
    {
        int sz = recv(sock, buf, BUF_SIZE, 0);
        if(sz > 0)
        {
            memcpy(res + offset, buf, sz);
            offset += sz;
        }
        else
        {
            break;
        }
    }
    res[offset] = '\0';
}


int is_success(const char *res)
{
    if(memcmp(res, "SUCCESS", 7) == 0)
        return 1;
    else
        return 0;
}


void display_help()
{
    printf("Available commands :\n");
    printf("   buy\n");
    printf("   sell\n");
    printf("   orders\n");
    printf("   trades\n");
    printf("   help\n");
    printf("   exit\n");
}


int authenticate(char *res)
{
    char req[MAX_REQ_SIZE];
    req[0] = '\0';
    add_field(req, "LOGIN");
    add_auth_fields(req);
    end_request(req);
    request(req, res);
    if(is_success(res))
        return 1;
    else
        return 0;
}


int trade_request(int type)
{
    int item_code, qty, unit_price;
    char req[MAX_REQ_SIZE];
    char res[MAX_RES_SIZE];
    char fld[MAX_FIELD_SIZE];
    req[0] = '\0';

    printf("    Item code  : ");
    scanf("%d", &item_code);
    printf("    Quantity   : ");
    scanf("%d", &qty);
    printf("    Unit Price : ");
    scanf("%d", &unit_price);

    add_field(req, ((type==0)?("BUY"):("SELL")) );
    add_auth_fields(req);
    sprintf(fld, "%d", item_code);
    add_field(req, fld);
    sprintf(fld, "%d", qty);
    add_field(req, fld);
    sprintf(fld, "%d", unit_price);
    add_field(req, fld);
    end_request(req);
    request(req, res);
    if(is_success(res))
        return 1;
    else
        return 0;
}


void view_request(int type)
{
    char req[MAX_REQ_SIZE];
    char res[MAX_RES_SIZE];
    req[0] = '\0';
    add_field(req, ((type==0)?("VIEW_ORDERS"):("VIEW_TRADES")) );
    add_auth_fields(req);
    end_request(req);
    request(req, res);
    if(!is_success(res))
    {
        printf("Request Failed\n");
        return;
    }
    printf("%s", seek_res(res));
}
