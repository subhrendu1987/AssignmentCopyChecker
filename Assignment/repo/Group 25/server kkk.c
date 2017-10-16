#include "stdio.h"
#include <stdlib.h>
#include "netdb.h"
#include "netinet/in.h"
#include "string.h"
#include "server.h"


void Socket_write( int fd_socket_write, char *message )
{
    int var1 = write( fd_socket_write, message, strlen(message) );
    if (var1 < 0) {
      // printing error 
      perror("ERROR writing to socket");         
    }
}

// splitting the buffer into tokens
char **split(char *buffer)
{
   int var1 = strlen(buffer), i, tokens=1;
   for(i=0; i<var1; i++)
      if(buffer[i]=='\n')
         tokens++;
   
   char *tok;
   char **res;
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

// 1 means username and password are both correct i.e matches with the username and password in login.txt file
// 2 means username is correct but password is wrong
// 3 means both are incorrect
int authValid( char *username, char *pass, int *userNum )
{
    FILE *file_point = fopen("login.txt", "r");
    char buffer_for_token[50];
    // usernum is not in use right now but later it is going to be used
    *userNum=0;
    while( fgets(  buffer_for_token, sizeof(buffer_for_token), file_point) != NULL )
    {
        *userNum += 1;
        char *username_local = strtok(buffer_for_token, ":");
        char *password_local = strtok(NULL, "\n");
        if( strcmp( username, username_local) == 0 )
        {
            if( strcmp(pass, password_local)==0)
               return 1;
            else 
               return 2;
        }
    }
    return 3;
}

// utility functions

// request comparing 
int req_than_req(struct request left_l, struct request right_r) {
   if(left_l.price < right_r.price) return 1;
   if(left_l.price == right_r.price) {
      return (left_l.id < right_r.id);
   }
   return 0;
}

// request swapping
void swap_requests(struct request *adam, struct request *eve) {
   struct request temp;
   temp = *adam;
   *adam = *eve;
   *eve = temp;
}

// sorted order insertion of the requests into the right queue
void sortInsert(struct request incoming_request) {
   struct request *new_array = ( (incoming_request.type=='B')?(Queue_buy[incoming_request.productNumber]):(Queue_sell[incoming_request.productNumber]) );
   int *head = ( (incoming_request.type == 'B') ? (Head_buy + incoming_request.productNumber) : (Head_sell + incoming_request.productNumber) );
   int *tail = ( (incoming_request.type == 'B') ? (Tail_buy + incoming_request.productNumber) : (Tail_sell + incoming_request.productNumber) );
   
   // check for empty queue
   if(*head == *tail) {
      new_array[*head] = incoming_request;
      *tail = (*tail + 1)%500;
   }
   else {
      int n_id = *head, i;
      while(!req_than_req(incoming_request, new_array[n_id]) && n_id != *tail) {
         n_id = (n_id+1)%500;
      }
      // insert element at n_id
      // shift all elements from n_id forward
      struct request temp = incoming_request;
      for(i = n_id; i != *tail; i = (i+1)%500 ) {
         swap_requests(new_array+i, &temp);
      }
      new_array[*tail] = temp;
      *tail = (*tail + 1)%500;
   }
   
}

// checking the presence of sequence of bytes in memory buffer
int memsubstr(const char *buf, int size_len, const char *seq) {
   int i;
   for(i=0; i+strlen(seq) < size_len ; i++) {
      if(memcmp(buf + i, seq, strlen(seq)) == 0) {
         return 1;
      }
   }
   return 0;
}


int main( int argc, char *argv[] ) {
   int fd_sock, new_fd_sock, num_port, len_client;
   char buffer[65536];
   struct sockaddr_in serv_addr, addr_client;
   int  n;
   // the localhost , the residing computer
   char ip[] = "127.0.0.1";  

   if(argc<2)
   {
      fprintf(stderr,"usage %s port\n", argv[0]);
      exit(0);
   }

   /* First call to socket() function */
   fd_sock = socket(AF_INET, SOCK_STREAM, 0);
   
   // socket opening 
   if (fd_sock < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   num_port = atoi(argv[1]);
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(num_port);
   // initiialization over

   /* Now bind the host address using bind() call.*/
   /*There are 65535 IP ports, but a server usually processes requests that come in on only one of them.
   It is like telling the phone room operator that we are now at work and available to answer the phone at a specific extension.
   We use bind(2) to tell sockets which port we want to serve.*/
   if (bind(fd_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will go in sleep mode and will wait for the incoming connection */
   /*In here, the backlog variable (i.e. here 5) tells sockets how many incoming requests to accept while you are busy processing the last request.
   In other words, it determines the maximum size of the queue of pending connections.
   */
   listen(fd_sock,5);
   len_client = sizeof(addr_client);
   
   while(1)
   {
        /* Accept actual connection from the client */
        new_fd_sock = accept(fd_sock, (struct sockaddr *)&addr_client, &len_client);
        if (new_fd_sock < 0) {
            perror("ERROR accepting");
            continue;
        }
      
        /* If connection is established then start communicating */
        char temp_buffer[256];
        int offset = 0;
        
        while(1) {
            memset(temp_buffer, '\0', 256);
            int var_sz = read( new_fd_sock, temp_buffer, 256, 0);
            if(var_sz < 0) {
                printf("Error reading from socket\n");
                exit(-1);
        }
        memcpy(buffer+offset, temp_buffer, var_sz);
        offset += var_sz;
        if(memsubstr(temp_buffer, 256, "$$")) break;
    }
    buffer[offset] = '\0';
      
      
      
      
    printf("Here is the message:\n%s\n",buffer);
    char **message = split(buffer);
      
      
    if(strcmp(message[2], "LOGIN")==0)
    {
        
        int userNum;
        int res = authValid(message[0], message[1], &userNum);
        switch( res )
        {
            case 1:     
                Socket_write( new_fd_sock, "SUCCESS\nLOG IN Successful!\n") ;
                break;
            case 2:     
                Socket_write(new_fd_sock, "FAIL\nWrong Password!!!\nTry again\n");
                break;
            case 3: 
                Socket_write(new_fd_sock, "FAIL\nWrong username!!!\nTry again\n");
                break;
        }
    }
    else if(strcmp(message[2], "BUY")==0)
    {
        int userNum;
        int res = authValid(message[0], message[1], &userNum);
        if(res != 1)
        {
            Socket_write( new_fd_sock, "FAIL\nLOG IN UnSuccessful!\n") ;
            close(new_fd_sock);
            continue;
        }
        /*construct the buy request structure*/
        int i;
        int product = atoi( message[3] );
        int quantity =  atoi( message[4] );
        int unitPrice = atoi( message[5] );
        struct request buy;
        strcpy(buy.user, message[0]);
        buy.productNumber = product;
        buy.quantity = quantity;
        buy.price = unitPrice;
        buy.id = requestID++;
        buy.type = 'B';
        //add to the initial user request log
        userRequests[userNum][ nRequest[ userNum ]++ ] = buy;
        //check if you get at least one seller
        int check =0;
        for(i=Head_sell[product]; i != Tail_sell[product]; i = (i+1)%500)           //check for each pending sell request
        {
            if( Queue_sell[product][i].price <= unitPrice )                     //if price is compatible
            {
                if( Queue_sell[product][i].quantity >= buy.quantity )                     //if the product can be fully purchased
                {
                    check = 1;
                    Queue_sell[product][i].quantity -= buy.quantity;                      //update the initial sell request
                    if(Queue_sell[product][i].quantity == 0)                         //if sell quantity is zero remove the request
                        Head_sell[product] = (Head_sell[product]+1)%500;
                    
                    /*update the transaction log*/
                    
                    struct userLog tempLog; strcpy(tempLog.buyer, message[0]); strcpy(tempLog.seller, Queue_sell[product][i].user); tempLog.productNumber = product; 
                    tempLog.price = Queue_sell[product][i].price; tempLog.quantity = buy.quantity; tempLog.buyRequestID = buy.id; 
                    tempLog.sellRequestID = Queue_sell[product][i].id; tradeLog[nTradeLog++] = tempLog;
                    break;
                }
                else                                                        //the sell quantity is less than that of buy
                {
                    Head_sell[product] = (Head_sell[product]+1)%500;                //remove from sell queue
                    buy.quantity -= Queue_sell[product][i].quantity;                      //update initial request
                    /*update the transaction log*/
                    struct userLog tempLog; strcpy(tempLog.buyer, message[0]); strcpy(tempLog.seller, Queue_sell[product][i].user); tempLog.productNumber = product; 
                    tempLog.price = Queue_sell[product][i].price; tempLog.quantity = Queue_sell[product][i].quantity; tempLog.buyRequestID = buy.id; 
                    tempLog.sellRequestID = Queue_sell[product][i].id; tradeLog[nTradeLog++] = tempLog;
                }
            }
        }
        
        //if no seller found, insert in buy queue
        if(check == 0)
            sortInsert(buy);
        
        Socket_write(new_fd_sock, "SUCCESS\n");
    }
    else if(strcmp(message[2], "SELL")==0)
    {
        int userNum;
        int res = authValid(message[0], message[1], &userNum);
        if(res != 1)
        {
            Socket_write( new_fd_sock, "FAIL\nLOG IN UnSuccessful!\n") ;
            close(new_fd_sock);
            continue;
        }
        //construct initial sell request
        int i;
        int product = atoi( message[3] );
        int quantity =  atoi( message[4] );
        int unitPrice = atoi( message[5] );
        struct request sell;
        strcpy(sell.user, message[0]);
        sell.productNumber = product;
        sell.quantity = quantity;
        sell.price = unitPrice;
        sell.id = requestID++;
        sell.type = 'S';
        //add to user request log
        userRequests[userNum][ nRequest[ userNum ]++ ] = sell;
        
        while( sell.quantity > 0 )
        {
            if(Head_buy[product] == Tail_buy[product])          //if no buyer available, add to sell queue
            {
                sortInsert(sell);
                break;
            }
            int Sell_best = Head_buy[product];
            //find index in buy queue with highest price in FCFS manner
            for(i=Head_buy[product]+1; i != Tail_buy[product]; i = (i+1)%500)       //check for each sell request pending
            {
                if( (Queue_buy[product][i].price > Queue_buy[product][Sell_best].price) || ( (Queue_buy[product][i].price == Queue_buy[product][Sell_best].price) && (Queue_buy[product][i].id < Queue_buy[product][Sell_best].id) ) )
                    Sell_best = i;
            }
            if( Queue_buy[product][Sell_best].price >= sell.price )              //if the price is compatible
            {
                if( Queue_buy[product][Sell_best].quantity > sell.quantity )               //sell whole amount in request
                {
                    Queue_buy[product][Sell_best].quantity -= sell.quantity;               //update the buy queue quantity
                    /*construct the transaction and update*/
                    struct userLog tempLog; strcpy(tempLog.seller, message[0]); strcpy(tempLog.buyer, Queue_buy[product][Sell_best].user); tempLog.productNumber = product; 
                    tempLog.price = sell.price; tempLog.quantity = sell.quantity; tempLog.buyRequestID = Queue_buy[product][Sell_best].id; 
                    tempLog.sellRequestID = sell.id; tradeLog[nTradeLog++] = tempLog;
                    
                    sell.quantity = 0;
                    break;
                }
                else                                                        //sell request still remains
                {
                    sell.quantity -= Queue_buy[product][Sell_best].quantity;               //update sell request quantity
                    /*update the trransaction log*/
                    struct userLog tempLog; strcpy(tempLog.seller, message[0]); strcpy(tempLog.buyer, Queue_buy[product][Sell_best].user); tempLog.productNumber = product; 
                    tempLog.price = sell.price; tempLog.quantity = Queue_buy[product][Sell_best].quantity; tempLog.buyRequestID = Queue_buy[product][Sell_best].id; 
                    tempLog.sellRequestID = sell.id; tradeLog[nTradeLog++] = tempLog;
                    //remove the entry form buy queue
                    for( i=Sell_best; i!=Tail_buy[product]; i=(i+1)%500 )
                        Queue_buy[product][i] = Queue_buy[product][(i+1)%500];
                    Tail_buy[product] = (500+Tail_buy[product] - 1)%500;
                }
            }
            else                                                            //price not compatible
            {
                sortInsert(sell);                                           //add to sell queue
                break;
            }
        }
        Socket_write(new_fd_sock, "SUCCESS\n");
    }
    /*display best sell and best buy for each product*/
    else if(strcmp(message[2], "VIEW_ORDERS")==0)
    {
        int i;
        char message[500] = "\0";
        for(i=0; i<10; i++)
        {
            sprintf(message+strlen(message), "Item: %d\n", i);
            strcpy(message+strlen(message), "   best sell: ");
            if( Head_sell[i] != Tail_sell[i] )
            {
                sprintf(message+strlen(message), "quantity- %d",Queue_sell[i][Head_sell[i]].quantity );
                strcpy(message+strlen(message), ", ");
                sprintf(message+strlen(message), "price- %d", Queue_sell[i][Head_sell[i]].price);
            }
            else
                strcpy(message+strlen(message), "NA");
            strcpy(message+strlen(message), "\n");
              
            strcpy(message+strlen(message), "   best Buy: ");
            if( Head_buy[i] != Tail_buy[i] )
            {  
                sprintf(message+strlen(message), "quantity- %d", Queue_buy[i][(500+Tail_buy[i]-1)%500].quantity);
                strcpy(message+strlen(message), ",");
                sprintf(message+strlen(message), "price- %d", Queue_buy[i][(500 + Tail_buy[i]-1)%500].price);
            }
            else
                strcpy(message+strlen(message), "NA");
            strcpy(message+strlen(message), "\n");
        }
        Socket_write(new_fd_sock, "SUCCESS\n");
        Socket_write(new_fd_sock, message);
    }  
    else if(strcmp(message[2], "VIEW_TRADES")==0)
    {
        int userNum;
        int res = authValid(message[0], message[1], &userNum);
        printf("%s: %d\n", message[0], nRequest[userNum]);
        if(res != 1)
        {
            Socket_write( new_fd_sock, "FAIL\nLOG IN UnSuccessful!\n") ;
            close(new_fd_sock);
            continue;
        }
        char Message_send[500];
        Socket_write(new_fd_sock, "SUCCESS\n");
        int i, j;
        for(j=0; j<nRequest[userNum]; j++)                                  //iterate in all initial requests of a given user
        {
            int getBuyID = userRequests[userNum][j].id;                     //get the buy id
            //print the initial request
            sprintf(Message_send, "INITIAL REQUEST\n");
            Socket_write(new_fd_sock, Message_send);
            sprintf(Message_send, "%s %d %d %d %c %d\n", userRequests[userNum][j].user, userRequests[userNum][j].productNumber, userRequests[userNum][j].quantity, userRequests[userNum][j].price, userRequests[userNum][j].type, userRequests[userNum][j].id);
            Socket_write(new_fd_sock, Message_send);
            sprintf(Message_send, "(BUYER SELLER ITEM PRICE QTY BUY_REQUEST_ID SELL_REQUEST_ID)\n");
            Socket_write(new_fd_sock, Message_send);
            for(i=0; i<nTradeLog; i++)                                      //check each transaction log
            {
                if(tradeLog[i].buyRequestID == getBuyID)                    //user in question is buyer
                {
                    sprintf(Message_send, "%s %s %d %d %d %d %d\n", tradeLog[i].buyer, tradeLog[i].seller, tradeLog[i].productNumber, tradeLog[i].quantity, tradeLog[i].price, tradeLog[i].buyRequestID, tradeLog[i].sellRequestID);
                    Socket_write(new_fd_sock, Message_send);
                }
                else if(tradeLog[i].sellRequestID == getBuyID)              //user in question is seller
                {
                    sprintf(Message_send, "%s %s %d %d %d %d %d\n", tradeLog[i].buyer, tradeLog[i].seller, tradeLog[i].productNumber, tradeLog[i].quantity, tradeLog[i].price, tradeLog[i].buyRequestID, tradeLog[i].sellRequestID);
                    Socket_write(new_fd_sock, Message_send);
                }
            }
        }
    }  
    close(new_fd_sock);
   }
   return 0;
}