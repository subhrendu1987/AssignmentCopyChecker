// Networks Lab Assignment 4
// Problem Statement #7
// Group 25
// TCP Trading Client
// Compile: gcc -Wall -o client client2.c
// chmod +x client2
// Usage: ./client <server-ip_addr> <server-port>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <arpa/inet.h>
#include <sys/socket.h>


// Definitions
#define MAX_REQ_SIZE 50000
#define MAX_Command_SIZE 30
#define MAX_FIELD_SIZE 30
#define MAX_SIZE 101
#define BUF_SIZE 1024
#define MAX_RES_SIZE 50000

const char *end_del = "$$";

// Login details
// Globals
static char Id_Trader[MAX_SIZE];
static char password[MAX_SIZE];
static struct sockaddr_in server;

// Prototypes

void add_field(char *req, const char *field);
void end_request(char *req);
int is_success(const char *res);
char *seek_res(char *res);
void print_Help();
int buy_request(char *Command, int type);
void view_request(int type);
void clear_stdin();
void sockaddr_init(const char *ip_addr, int port);
void request(const char *req, char *res);
int authenticate(); 
void prepend_auth(char *req);

// Definitions
// initialize sockaddr structure
void sockaddr_init(const char *ip_addr, int port) {
    server.sin_addr.s_addr = inet_addr(ip_addr);
    // the one used for ip is AF_INET (it is a symbol for the constant 2)   
    // use the reference -- https://www.freebsd.org/doc/en/books/developers-handbook/sockets-essential-functions.html
    // #define AF_INET     2      -->>   internetwork: UDP, TCP, etc.
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
}

// Once a client has created a socket, it needs to connect it to a specific port on a remote system. It uses connect(2) or in this case request function heres
// send a plaintext request and fetch response
void request(const char *req, char *res) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        printf("Unable to create socket\n");
        exit(-1);
    }

    if( connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0 ) {
        printf("Unable to connect to server\n");
        exit(-1);
    }

    // printf("D: connected to server\n");
    // send request data
    if( send(sock, req, strlen(req), 0) < 0 ) {
        printf("Unable to send() to server\n");
        exit(-1);
    }

    // fetch response data
    char *buf[BUF_SIZE];
    int offset = 0;
    while( 1 ) {
        int sz = recv(sock, buf, BUF_SIZE, 0);
        if(sz > 0) {
            memcpy(res + offset, buf, sz);
            offset += sz;
        }
        else {
            break;
        }
    }

    res[offset] = '\0';

}

void add_field(char *req, const char *field) {
    strcat(req, field);
    strcat(req, "\n");
}

// prepends login details to request string
void prepend_auth(char *req) {
    add_field(req, Id_Trader);
    add_field(req, password);
}

// add special end delimiter to request string
void end_request(char *req) {
    strcat(req, end_del);
}

// checks if response is successful
int is_success(const char *res) {
    if(memcmp(res, "SUCCESS", 7) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

// seeks past a field in the response
char *seek_res(char *res) {
    char *p = res;
    while(*p != '\n') p++;
    p++;
    return p;
}

/* user authenticate*/
int authenticate(char *res) {
    char req[MAX_REQ_SIZE];
    req[0] = '\0';
    prepend_auth(req);
    add_field(req, "LOGIN");
    end_request(req);

    // send request
    request(req, res);
    // check response
    if(is_success(res)) return 1;
    else return 0;
}

// Help instructions
void print_Help() {
    printf("List of commands:\n");
    printf("orders\n");
    printf("buy\n");
    printf("Sell\n");
    printf("trades\n");
    printf("Help\n");
}

// sell or buy request
int bs_request(int item_code, int Quantity, int unit_price, int type) {
    char req[MAX_REQ_SIZE];
    char res[MAX_RES_SIZE];
    char fld[MAX_FIELD_SIZE];
    req[0] = '\0';
    prepend_auth(req);
    add_field(req, ((type==0)?("BUY"):("SELL")) );
    sprintf(fld, "%d", item_code);
    add_field(req, fld);
    sprintf(fld, "%d", Quantity);
    add_field(req, fld);
    sprintf(fld, "%d", unit_price);
    add_field(req, fld);
    end_request(req);

    // send request
    request(req, res);
    // check response
    if(is_success(res)) return 1;
    else return 0;
}

void view_request(int type) {
    char req[MAX_REQ_SIZE];
    char res[MAX_RES_SIZE];
    req[0] = '\0';
    prepend_auth(req);
    add_field(req, ((type==0)?("VIEW_ORDERS"):("VIEW_TRADES")) );
    end_request(req);

    // send request
    request(req, res);
    // check response
    if(!is_success(res)) {
        printf("Request Failed\n");
        return;
    }

    printf("%s", seek_res(res)); 
}


int main(int argc, char const *argv[])
{
    sockaddr_init(argv[1], atoi(argv[2]));

    // login prompt
    while( 1 ) {
        char res[MAX_RES_SIZE];
        printf("Trader id: ");
        scanf("%s", Id_Trader);
        printf("Password: ");
        scanf("%s", password);

        if(authenticate(res)) {
            printf("%s", seek_res(res));
            break;
        }
        else {
            printf("%s", seek_res(res)); 
        }
    }

    print_Help();

    // repl
    while( 1 ) {
        char Command[MAX_Command_SIZE];
        int item_code, Quantity, unit_price;
        printf("trading-sys> ");
        scanf("%s", Command);
        if(strcmp(Command, "") == 0) continue;
        else if(strcmp(Command, "buy") == 0) {
            printf("Item code: ");
            scanf("%d", &item_code);
            printf("Quantity: ");
            scanf("%d", &Quantity);
            printf("Unit Price: ");
            scanf("%d", &unit_price);
            bs_request(item_code, Quantity, unit_price, 0);
        }
        else if(strcmp(Command, "sell") == 0) {
            printf("Item code: ");
            scanf("%d", &item_code);
            printf("Quantity: ");
            scanf("%d", &Quantity);
            printf("Unit Price: ");
            scanf("%d", &unit_price);
            bs_request(item_code, Quantity, unit_price, 1);
        }
        else if(strcmp(Command, "orders") == 0) {
            view_request(0);
        }
        else if(strcmp(Command, "trades") == 0) {
            view_request(1);
        }
        else if(strcmp(Command, "Help") == 0) {
            print_Help();
        }
        else {
            printf("Invalid command\n");
        }
    }

    
        
    return 0;
}