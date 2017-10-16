// Compile: gcc client1.c -o client1
// Usage: ./client1 <server-ip> <server-port>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// func def
#define MAX_SIZE 101
#define BUF_SIZE 1024
#define MAX_RES_SIZE 65536
#define MAX_REQ_SIZE 65536
#define MAX_CMD_SIZE 32
#define MAX_FIELD_SIZE 32
const char *marker_end = "@@@";

// username and password
static char username[MAX_SIZE];
static char password[MAX_SIZE];
static struct sockaddr_in server;

void request(const char *req, char *res);
char *print_response(char *res);
void view_request(int type);
int authentication(); 
void prefix(char *req);
void end_rqst(char *req);
void sockaddr_init(const char *ip, int port);
int ifsuccess(const char *res);
void print_help();
int buy_request(char *cmd, int type);
void clear_stdin();
void append(char *req, const char *field);


void sockaddr_init(const char *ip, int port)
 {
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
}

// sending text request and fetching the response
void request(const char *req, char *res) 
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        printf("Creating socket failed!\n");
        exit(-1);
    }

    if( connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0 ) 
    {
        printf("Coudln't connect to server!\n");
        exit(-1);
    }

    //printf("Connected to server\n");
    // sending request
    if( send(sock, req, strlen(req), 0) < 0 )
    {
        printf("Couldn't send request to the server!\n");
        exit(-1);
    }

    // fetch response data
    char *buf[BUF_SIZE];
    int offset = 0;
    while( 1 ) 
	{
           int count_s = recv(sock, buf, BUF_SIZE, 0);
           if(count_s > 0) {
           memcpy(res+offset, buf, count_s);
           offset += count_s;
        }
        else 
	{
            break;
        }
    }
          res[offset] = '\0';

}


// add login credentials to request 
void prefix(char *req) 
{
    append (req, username);
    append (req, password);
}


void append(char *req, const char *field)
 {
    strcat (req, field);
    strcat (req, "\n");
}

// add string end marker
void end_rqst (char *req) 
{
    strcat (req, marker_end);
}

// printing response
char *print_response(char *res) 
{
    char *s = res;
    while(*s != '\n') s++;
    s++;
    return s;
}

// checking if success
int ifsuccess(const char *res) 
{
    if(memcmp(res, "SUCCESS", 7) == 0)
 {
        return 1;
    }
    else {
        return 0;
    }
}


// authentication credentials of user
int authentication(char *res) 
{
    char req[MAX_REQ_SIZE];
    req[0] = '\0';
    
    prefix(req);
    append(req, "LOGIN");
    end_rqst(req);

    // send request
    request(req, res);
    // check response
    if(ifsuccess(res)) return 1;
    else return 0;
}

void print_help() {
    printf("\nOptions:\n");
    printf("orders\n");
    printf("buy\n");
    printf("sell\n");
    printf("trades\n");
    
}

// buy sell req
int bs_request(int item_code, int qty, int unit_price, int type)
 {
    char req[MAX_REQ_SIZE];
    char res[MAX_RES_SIZE];
    char fld[MAX_FIELD_SIZE];

    req[0] = '\0';
    prefix(req);

    append(req, ((type==0)?("BUY"):("SELL")) );
    sprintf(fld, "%d", item_code);

    append(req, fld);
    sprintf(fld, "%d", qty);

    append(req, fld);
    sprintf(fld, "%d", unit_price);

    append(req, fld);
    end_rqst(req);

    // sending request
    request(req, res);

    // checking response
    if(ifsuccess(res)) return 1;
    
    else return 0;
}

void view_request(int type)
 {
    char req[MAX_REQ_SIZE];
    char res[MAX_RES_SIZE];

    req[0] = '\0';
    prefix(req);
    append(req, ((type==0)?("VIEW_ORDERS"):("VIEW_TRADES")) );
    end_rqst(req);

    // sending request
    request(req, res);
    // checking response
    if(!ifsuccess(res))
 {
        printf("Request Failed\n");
        return;
    }

    printf("%s", print_response(res)); 
}


int main(int argc, char const *argv[])
{
    sockaddr_init(argv[1], atoi(argv[2]));

    // login 
    while( 1 ) {
        char res[MAX_RES_SIZE];

        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);

        if(authentication(res))
	 {
            printf("%s", print_response(res));
            break;
        }
        else 
	{
            printf("%s", print_response(res)); 
        }
    }

    print_help();

    // repl
    while( 1 ) {
        char cmd[MAX_CMD_SIZE];
        int item_code;
	int qty;
	int unit_price;

        printf("\nEnter option: ");
        scanf("%s", cmd);

        if(strcmp(cmd, "") == 0) continue;
        else if(strcmp(cmd, "buy") == 0)
	 {
            printf("Item_id: ");
            scanf("%d", &item_code);
            printf("Quantity: ");
            scanf("%d", &qty);
            printf("Price per unit: ");
            scanf("%d", &unit_price);
            bs_request(item_code, qty, unit_price, 0);
        }
        else if(strcmp(cmd, "sell") == 0)
	 {
            printf("Item_id: ");
            scanf("%d", &item_code);
            printf("Quantity: ");
            scanf("%d", &qty);
            printf("Price per unit: ");
            scanf("%d", &unit_price);
            bs_request(item_code, qty, unit_price, 1);
        }
        else if(strcmp(cmd, "orders") == 0) 
	{
            view_request(0);
        }
        else if(strcmp(cmd, "trades") == 0) 
	{
            view_request(1);
        }
        
        else 
	{
            printf("Invalid command\n");
        }
    }

    
        
    return 0;
}
