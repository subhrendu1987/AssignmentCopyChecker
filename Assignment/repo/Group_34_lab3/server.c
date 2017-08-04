#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>

#define MAX_USERS 5
#define MAX_ITEMS 10
#define MAX_REQUESTS 100
#define MAX_TRANSACTIONS MAX_USERS*MAX_REQUESTS


typedef struct request
{
    int id;
    char client_id[20];
    int item_id, quantity, price;
    char type;
} request;

typedef struct transaction
{
    char buyer[20];
    char seller[20];
    int item_id, quantity, price;
    int buy_request_id, sell_request_id;
} transaction;


const char *request_delimeter = "**********";
request user_requests[MAX_USERS][MAX_REQUESTS];
request buy_queue[MAX_ITEMS][MAX_TRANSACTIONS];
request sell_queue[MAX_ITEMS][MAX_TRANSACTIONS];
transaction trade_log[MAX_TRANSACTIONS];
int user_wise_request_count[MAX_USERS] = {0};
int trade_log_count = 0, request_count = 0;
int buy_queue_head[MAX_ITEMS]={0}, buy_queue_tail[MAX_ITEMS]={0};
int sell_queue_head[MAX_ITEMS]={0}, sell_queue_tail[MAX_ITEMS]={0};

void write_socket( int sockfd, char *msg );
int find_bytes_in_mem_buffer(const char *buf, int len, const char *seq);
char **split(char *buffer);
int login_status( char *client_id, char *client_password, int *user_num );
int r_less_than_r(request l, request r);
void swap_requests(request *a, request *b);
void insert_request(request r);
int handle_query(int newsockfd, char **msg);


int main( int argc, char *argv[] )
{
    char choice;
    int sockfd, newsockfd, server_port, client_addr_len;
    char buffer[65536];
    struct sockaddr_in server_addr, client_addr;

    if (argc!=2)
    {
        printf("Usage:\n");
        printf("./server <port>\n");
        printf("---------------\n");
        printf("Run server on port 1234? (Y/n) : ");
        scanf("%c",&choice);
        if(choice!='\n' && choice!='Y' && choice!='y')
            exit(0);
        server_port = 1234;
    }
    else
    {
        server_port = atoi(argv[1]);
    }

    /* Create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0)
    {
        printf("ERROR : Unable to create socket\n");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server_port);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        printf("ERROR : Unable to bind socket\n");
        exit(1);
    }

    /*
        Now start listening for the clients, here process will
        go in sleep mode and will wait for the incoming connection
    */
    listen(sockfd,5);
    client_addr_len = sizeof(client_addr);

    printf("Server running on port %d\n", server_port);
    printf("---------------------------\n");

    while(1)
    {
        /* Accept actual connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            continue;
        }

        /* If connection is established then start communicating */
        char buff[256];
        int offset = 0;

        while(1)
        {
            memset(buff, '\0', 256);
            int sz = read( newsockfd, buff, 256, 0);
            if(sz < 0)
            {
                printf("Error reading from socket\n");
                exit(-1);
            }
            memcpy(buffer+offset, buff, sz);
            offset += sz;
            if(find_bytes_in_mem_buffer(buff, 256, request_delimeter)) break;
        }
        buffer[offset] = '\0';
        char **msg = split(buffer);

        // printf("Received message :\n%s\n",buffer);
        handle_query(newsockfd, msg);
        printf("--------------------\n");

        close(newsockfd);
    }

   return 0;
}


void write_socket( int sockfd, char *msg )
{
    int n = write( sockfd, msg, strlen(msg) );
    if (n < 0)
    {
        perror("ERROR writing to socket");
    }
}

int find_bytes_in_mem_buffer(const char *buf, int len, const char *seq)
{
    int i;
    for(i=0; i+strlen(seq) < len ; i++)
    {
        if(memcmp(buf + i, seq, strlen(seq)) == 0)
            return 1;
    }
    return 0;
}

char **split(char *buffer)
{
    int l = strlen(buffer), i, tokens=1;
    char *tok;
    char **res;
    for(i=0; i<l; i++)
    {
        if(buffer[i]=='\n')
            tokens++;
    }
    res = (char **)malloc(tokens * sizeof(char *));
    tok = strtok(buffer, "\n");
    i=0;
    while(tok!=NULL)
    {
        res[i] = (char *)malloc(sizeof(char)*strlen(tok));
        strcpy(res[i], tok);
        i++;
        tok = strtok(NULL, "\n");
    }
    return res;
}

int login_status( char *client_id, char *client_password, int *user_num )
{
    FILE *fp = fopen("login.txt", "r");
    char buff[50];
    *user_num=0;
    while( fgets(  buff, sizeof(buff), fp) != NULL )
    {
        *user_num += 1;
        char *user1 = strtok(buff, ":");
        char *pass1 = strtok(NULL, "\n");
        if( strcmp( client_id, user1) == 0 )
        {
            if( strcmp(client_password, pass1)==0)
               return 1;
            else
               return 2;
        }
    }
    return 3;
}

int r_less_than_r(request l, request r)
{
    if(l.price < r.price)
        return 1;
    if(l.price == r.price)
        return (l.id < r.id);
    return 0;
}

void swap_requests(request *a, request *b)
{
    request temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void insert_request(request req)
{
    request *arr = ( (req.type=='B')?(buy_queue[req.item_id]):(sell_queue[req.item_id]) );
    int *head = ( (req.type == 'B') ? (buy_queue_head + req.item_id) : (sell_queue_head + req.item_id) );
    int *tail = ( (req.type == 'B') ? (buy_queue_tail + req.item_id) : (sell_queue_tail + req.item_id) );
    if(*head == *tail)
    {
        arr[*head] = req;
        *tail = (*tail + 1)%500;
    }
    else
    {
        int idx = *head, i;
        while(!r_less_than_r(req, arr[idx]) && idx != *tail)
        {
            idx = (idx+1)%500;
        }
        request temp = req;
        for(i = idx; i != *tail; i = (i+1)%500 )
        {
            swap_requests(arr+i, &temp);
        }
        arr[*tail] = temp;
        *tail = (*tail + 1)%500;
    }
}


int handle_query(int newsockfd, char **msg)
{
    if(strcmp(msg[0], "LOGIN")==0)
    {
        printf("Query  : LOGIN attempt for client_id %s\n", msg[1]);
        int user_num;
        int res = login_status(msg[1], msg[2], &user_num);
        switch( res )
        {
            case 1:
                printf("Status : LOGIN successful\n");
                write_socket(newsockfd, "SUCCESS\nLOGIN Successful !\n");      
                break;
            case 2:
                printf("Status : LOGIN failed\n");
                write_socket(newsockfd, "FAIL\nWrong Password !\nTry again\n");
                break;
            case 3:
                printf("Status : LOGIN failed\n");
                write_socket(newsockfd, "FAIL\nWrong username !\nTry again\n");
                break;
        }
    }
    else if(strcmp(msg[0], "BUY")==0)
    {
        printf("Query  : BUY request by client_id %s\n", msg[1]);
        int user_num;
        int res = login_status(msg[1], msg[2], &user_num);
        if(res != 1)
        {
            printf("Status : LOGIN failed\n");
            write_socket( newsockfd, "FAIL\nLogin Unsuccessful!\n") ;
            return 0;
        }

        int i;
        int item = atoi( msg[3] );
        int qty =  atoi( msg[4] );
        int unitPrice = atoi( msg[5] );
        request buy;
        strcpy(buy.client_id, msg[1]);
        buy.item_id = item;
        buy.quantity = qty;
        buy.price = unitPrice;
        buy.id = request_count++;
        buy.type = 'B';

        user_requests[user_num][ user_wise_request_count[ user_num ]++ ] = buy;

        int check =0;
        for(i=sell_queue_head[item]; i != sell_queue_tail[item]; i = (i+1)%500)
        {
            if( sell_queue[item][i].price <= unitPrice )
            {
                if( sell_queue[item][i].quantity >= buy.quantity )
                {
                    check = 1;
                    sell_queue[item][i].quantity -= buy.quantity;
                    if(sell_queue[item][i].quantity == 0)
                        sell_queue_head[item] = (sell_queue_head[item]+1)%500;
                    transaction tempLog; strcpy(tempLog.buyer, msg[1]); strcpy(tempLog.seller, sell_queue[item][i].client_id); tempLog.item_id = item;
                    tempLog.price = sell_queue[item][i].price; tempLog.quantity = buy.quantity; tempLog.buy_request_id = buy.id;
                    tempLog.sell_request_id = sell_queue[item][i].id; trade_log[trade_log_count++] = tempLog;
                    break;
                }
                else
                {
                    sell_queue_head[item] = (sell_queue_head[item]+1)%500;
                    buy.quantity -= sell_queue[item][i].quantity;
                    transaction tempLog; strcpy(tempLog.buyer, msg[1]); strcpy(tempLog.seller, sell_queue[item][i].client_id); tempLog.item_id = item;
                    tempLog.price = sell_queue[item][i].price; tempLog.quantity = sell_queue[item][i].quantity; tempLog.buy_request_id = buy.id;
                    tempLog.sell_request_id = sell_queue[item][i].id; trade_log[trade_log_count++] = tempLog;
                }
            }
        }
        if(check == 0)
            insert_request(buy);
        write_socket(newsockfd, "SUCCESS\n");
        printf("Status : Buy request added\n");
    }
    else if(strcmp(msg[0], "SELL")==0)
    {
        printf("Query  : SELL request by client_id %s\n", msg[1]);
        int user_num;
        int res = login_status(msg[1], msg[2], &user_num);
        if(res != 1)
        {
            printf("Status : LOGIN failed\n");
            write_socket( newsockfd, "FAIL\nLogin Unsuccessful!\n") ;
            return 0;
        }

        int i;
        int item = atoi( msg[3] );
        int qty =  atoi( msg[4] );
        int unitPrice = atoi( msg[5] );
        request sell;
        strcpy(sell.client_id, msg[1]);
        sell.item_id = item;
        sell.quantity = qty;
        sell.price = unitPrice;
        sell.id = request_count++;
        sell.type = 'S';

        user_requests[user_num][ user_wise_request_count[ user_num ]++ ] = sell;

        while( sell.quantity > 0 )
        {
            if(buy_queue_head[item] == buy_queue_tail[item])
            {
                insert_request(sell);
                break;
            }
            int bestSell = buy_queue_head[item];

            for(i=buy_queue_head[item]+1; i != buy_queue_tail[item]; i = (i+1)%500)
            {
                if( (buy_queue[item][i].price > buy_queue[item][bestSell].price) || ( (buy_queue[item][i].price == buy_queue[item][bestSell].price) && (buy_queue[item][i].id < buy_queue[item][bestSell].id) ) )
                    bestSell = i;
            }
            if( buy_queue[item][bestSell].price >= sell.price )
            {
                if( buy_queue[item][bestSell].quantity > sell.quantity )
                {
                    buy_queue[item][bestSell].quantity -= sell.quantity;
                    transaction tempLog; strcpy(tempLog.seller, msg[1]); strcpy(tempLog.buyer, buy_queue[item][bestSell].client_id); tempLog.item_id = item;
                    tempLog.price = sell.price; tempLog.quantity = sell.quantity; tempLog.buy_request_id = buy_queue[item][bestSell].id;
                    tempLog.sell_request_id = sell.id; trade_log[trade_log_count++] = tempLog;
                    sell.quantity = 0;
                    break;
                }
                else
                {
                    sell.quantity -= buy_queue[item][bestSell].quantity;
                    transaction tempLog; strcpy(tempLog.seller, msg[1]); strcpy(tempLog.buyer, buy_queue[item][bestSell].client_id); tempLog.item_id = item;
                    tempLog.price = sell.price; tempLog.quantity = buy_queue[item][bestSell].quantity; tempLog.buy_request_id = buy_queue[item][bestSell].id;
                    tempLog.sell_request_id = sell.id; trade_log[trade_log_count++] = tempLog;
                    for( i=bestSell; i!=buy_queue_tail[item]; i=(i+1)%500 )
                    {
                        buy_queue[item][i] = buy_queue[item][(i+1)%500];
                    }
                    buy_queue_tail[item] = (500+buy_queue_tail[item] - 1)%500;
                }
            }
            else
            {
                insert_request(sell);
                break;
            }
        }
        write_socket(newsockfd, "SUCCESS\n");
        printf("Status : Sell request added\n");
    }
    else if(strcmp(msg[0], "VIEW_ORDERS")==0)
    {
        printf("Query  : VIEW_ORDERS request by client_id %s\n", msg[1]);
        int i;
        char msg[500] = "\0", temp[500] = "\0";

        write_socket(newsockfd, "SUCCESS\n");
        sprintf(msg, "  Item Id  |  Best sell price  |  Best buy price \n");
        write_socket(newsockfd, msg);
        sprintf(msg, " --------- + ----------------- + ----------------\n");
        write_socket(newsockfd, msg);
        for(i=0; i<10; i++)
        {
            sprintf(msg, "%10d | ", i);
            write_socket(newsockfd, msg);
            if( sell_queue_head[i] != sell_queue_tail[i] )
                sprintf(temp, "%d (qty : %d)", sell_queue[i][sell_queue_head[i]].price, sell_queue[i][sell_queue_head[i]].quantity);
            else
                strcpy(temp, "N/A");
            sprintf(msg, "%-17s | ", temp );
            write_socket(newsockfd, msg);
            if( buy_queue_head[i] != buy_queue_tail[i] )
                sprintf(temp, "%d (qty : %d)", buy_queue[i][(500 + buy_queue_tail[i]-1)%500].price, buy_queue[i][(500+buy_queue_tail[i]-1)%500].quantity);
            else
                strcpy(temp, "N/A");
            sprintf(msg, "%-16s\n", temp );
            write_socket(newsockfd, msg);
        }
        printf("Status : Orders data returned\n");
    }
    else if(strcmp(msg[0], "VIEW_TRADES")==0)
    {
        printf("Query  : VIEW_TRADES request by client_id %s\n", msg[1]);
        int user_num;
        int res = login_status(msg[1], msg[2], &user_num);
        if(res != 1)
        {
            printf("Status : LOGIN failed\n");
            write_socket( newsockfd, "FAIL\nLogin Unsuccessful!\n") ;
            return 0;
        }
        char sendMsg[500];
        write_socket(newsockfd, "SUCCESS\n");
        int i, j;
        if(user_wise_request_count[user_num]==0)
        {
            sprintf(sendMsg, "You do not have any transaction yet\n");
            write_socket(newsockfd, sendMsg);
        }
        else
        {
            sprintf(sendMsg, "Your requests :\n");
            write_socket(newsockfd, sendMsg);
            for(j=0; j<user_wise_request_count[user_num]; j++)
            {
                int getBuyID = user_requests[user_num][j].id;
                sprintf(sendMsg, "  %d)\n", j+1);
                write_socket(newsockfd, sendMsg);
                sprintf(sendMsg, "    Request Id   : %d\n", user_requests[user_num][j].id);
                write_socket(newsockfd, sendMsg);
                sprintf(sendMsg, "    Request Type : %c\n", user_requests[user_num][j].type);
                write_socket(newsockfd, sendMsg);
                sprintf(sendMsg, "    Item Id      : %d\n", user_requests[user_num][j].item_id);
                write_socket(newsockfd, sendMsg);
                sprintf(sendMsg, "    Quantity     : %d\n", user_requests[user_num][j].quantity);
                write_socket(newsockfd, sendMsg);
                sprintf(sendMsg, "    Price        : %d\n", user_requests[user_num][j].price);
                write_socket(newsockfd, sendMsg);
                sprintf(sendMsg, "    Trades for this request :\n");
                write_socket(newsockfd, sendMsg);
                sprintf(sendMsg, "      BUYER | SELLER | ITEM |  PRICE  | QUANTITY | BUY_REQUEST_ID | SELL_REQUEST_ID\n");
                write_socket(newsockfd, sendMsg);
                sprintf(sendMsg, "      ----- + ------ + ---- + ------- + -------- + -------------- + ---------------\n");
                write_socket(newsockfd, sendMsg);
                for(i=0; i<trade_log_count; i++)
                {
                    if(trade_log[i].buy_request_id == getBuyID)
                    {
                        sprintf(sendMsg, "      %5s | %6s | %4d | %7d | %8d | %14d | %15d\n", trade_log[i].buyer, trade_log[i].seller, trade_log[i].item_id, trade_log[i].price, trade_log[i].quantity, trade_log[i].buy_request_id, trade_log[i].sell_request_id);
                        write_socket(newsockfd, sendMsg);
                    }
                    else if(trade_log[i].sell_request_id == getBuyID)
                    {
                        sprintf(sendMsg, "      %5s | %6s | %4d | %7d | %8d | %14d | %15d\n", trade_log[i].buyer, trade_log[i].seller, trade_log[i].item_id, trade_log[i].price, trade_log[i].quantity, trade_log[i].buy_request_id, trade_log[i].sell_request_id);
                        write_socket(newsockfd, sendMsg);
                    }
                }
            }
        }
        printf("Status : Transaction data returned\n");
    }
    return 1;
}
