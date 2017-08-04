// Compile: gcc server.c -o server
// Usage: ./server <server-port>

#include "stdio.h"
#include <stdlib.h>
#include "netdb.h"
#include "netinet/in.h"
#include "string.h"
#include "server.h"


void writeSocket( int sockfd, char *message )
{
    int p = write( sockfd, message, strlen(message) );
    if (p < 0) {
         perror("error in writing to socket");
      }
}

char **tokenize(char *stream)
{
   char *tok;
   char **res;

   int m = strlen(stream);
   int i;
   int tokens=1;

   for(i=0; i<m; i++)
      if(stream[i]=='\n')
         tokens++;
  
       i=0;
   res = (char **) malloc(tokens * sizeof(char *));
   tok = strtok(stream, "\n");
   

   while(tok!=NULL)
   {
      res[i] = (char *)malloc(sizeof(char)*strlen(tok));
      strcpy(res[i], tok);
      i++;
      tok = strtok(NULL, "\n");
   }
   return res;
}


int check_auth( char *username, char *pass, int *userNum )
{
    FILE *fp = fopen("login.txt", "r");
    char buff[50];
    *userNum=0;
    while( fgets(  buff, sizeof(buff), fp) != NULL )
    {
        *userNum += 1;
        char *user1 = strtok(buff, ":");
        char *pass1 = strtok(NULL, "\n");
        if( strcmp( username, user1) == 0 )
        {
            if( strcmp(pass, pass1)==0)
               return 1;
            else 
               return 2;
        }
    }
    return 3;
}

// compare requests
int rlessthanr(struct rqst l, struct rqst r)
 {
   if(l.price == r.price)
   {
      return (l.id < r.id);
   }

   if(l.price < r.price) 
    return 1;
   
   return 0;
}

// swap requests
void swap_rqst(struct rqst *first, struct rqst *second) {
   struct rqst temp;
   temp = *first;
   *first = *second;
   *second = temp;
}

// Insert request in sorted order into the rigth queue
void sort_insert(struct rqst req) {
   struct rqst *b = ( (req.type=='B')?(buy_q[req.item_num]):(sell_q[req.item_num]) );

   int *start = ( (req.type == 'B') ? (buy_start + req.item_num) : (sell_start + req.item_num) );
   int *end = ( (req.type == 'B') ? (buy_end + req.item_num) : (sell_end + req.item_num) );

   // empty queue?
   if(*start == *end)
 {
      b[*start] = req;
      *end = (*end + 1)%500;
   }
   else 
     {
      int pos = *start, i;
      while(!rlessthanr(req, b[pos]) && pos != *end) {
         pos = (pos+1)%500;
      }
      
      struct rqst temp = req;
      for(i = pos; i != *end; i = (i+1)%500 ) {
         swap_rqst(b+i, &temp);
      }
      b[*end] = temp;
      *end = (*end + 1)%500;
   }
   
}

// check is sequence of bytes is present in memory stream
int check_seq(const char *buf, int len, const char *seq) {
   int i;
   for(i=0; i+strlen(seq) < len ; i++) {
      if(memcmp(buf + i, seq, strlen(seq)) == 0) {
         return 1;
      }
   }
   return 0;
}


int main( int argc, char *argv[] )
{
   int n,sockfd, new_socket_fd, port_num, client_size;
   char stream[65536];
   struct sockaddr_in server_address, client_address;
   
   if(argc<2)
   {
      printf("usage <port>\n");
      exit(0);
   }

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0)
   {
      perror("Error in opening the socket!");
      exit(1);
   }
   
   /* socket structure */
   bzero((char *) &server_address, sizeof(server_address));
   port_num = atoi(argv[1]);
   
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = INADDR_ANY;
   server_address.sin_port = htons(port_num);

   if (bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
   {
      perror("Error in binding!");
      exit(1);
   }
      
   //listening to the socket
   listen(sockfd,5);
   client_size = sizeof(client_address);
   
   while(1)
   {
        
        new_socket_fd = accept(sockfd, (struct sockaddr *)&client_address, &client_size);
        if (new_socket_fd < 0)
        {
            perror("Error in accepting!");
            continue;
        }
      
        /* after connection establishment start communicating */
        char buff[256];
        int offset = 0;
        
        while(1) {
            memset(buff, '\0', 256);
            int s_counter = read( new_socket_fd, buff, 256, 0);
            if(s_counter < 0) 
            {
                printf("Socket reading failed\n");
                exit(-1);
              }
        memcpy(stream+offset, buff, s_counter);
        offset += s_counter;
        if(check_seq(buff, 256, "@@@")) break;
    }
    stream[offset] = '\0';
         
    printf("Query:\n%s\n",stream);
    char **message = tokenize(stream);
      
      
    if(strcmp(message[2], "LOGIN")==0)
    {
        int userNum;
        int res = check_auth(message[0], message[1], &userNum);
        switch( res )
        {
            case 1:     
                writeSocket( new_socket_fd, "SUCCESS\n *Successfully logged in*\n") ;
                break;
            case 2:     
                writeSocket(new_socket_fd, "FAIL\n Incorrect password!\n");
                break;
            case 3: 
                writeSocket(new_socket_fd, "FAIL\n Incorrect username!\n");
                break;
        }
    }


 
    else if(strcmp(message[2], "BUY")==0)
    {
        int userNum;
        int res = check_auth(message[0], message[1], &userNum);
        if(res != 1)
        {
            writeSocket( new_socket_fd, "FAIL\nLOG IN UnSuccessful!\n") ;
            close(new_socket_fd);
            continue;
        }
       
        int i;
        int item_id = atoi( message[3] );
        int qty =  atoi( message[4] );
        int price_unit = atoi( message[5] );

        struct rqst buy;
        strcpy(buy.user, message[0]);
        buy.item_num = item_id;
        buy.qty = qty;
        buy.price = price_unit;
        buy.id = req_num++;
        buy.type = 'B';

        
        req_user [userNum][ num_of_request[ userNum ]++ ] = buy;
        //check if you get at least one seller
        int check =0;
	//check for each pending sell request
        for(i=sell_start[item_id]; i != sell_end [item_id]; i = (i+1)%500)           
        {
	//if price is compatible
            if( sell_q[item_id][i].price <= price_unit )                     
            {	
	//if the item can be fully purchased
                if( sell_q[item_id][i].qty >= buy.qty )                     
                {
                    check = 1;
	//update the initial sell request
                    sell_q[item_id][i].qty -= buy.qty;
   
	//if sell qty is zero remove the request                   
                    if(sell_q[item_id][i].qty == 0)                         
                        sell_start[item_id] = (sell_start[item_id]+1)%500;
                    
                    struct userlog templog; 
			strcpy(templog.buyer, message[0]); 
			strcpy(templog.seller, sell_q[item_id][i].user); 
			templog.item_num = item_id; 
                    	templog.price = sell_q[item_id][i].price;
 		 	templog.qty = buy.qty; 
			templog.buy_req_id = buy.id; 
	                templog.sell_req_id = sell_q[item_id][i].id; 
			trade_data[trade_data_n++] = templog;
                    break;
                }
                else                                                       
                {
                    sell_start[item_id] = (sell_start[item_id]+1)%500;               
                    buy.qty -= sell_q[item_id][i].qty;               
                    
                    struct userlog templog; 
			strcpy(templog.buyer, message[0]);
			strcpy(templog.seller, sell_q[item_id][i].user); 
			templog.item_num = item_id; 
                        templog.price = sell_q[item_id][i].price; 
			templog.qty = sell_q[item_id][i].qty;
			templog.buy_req_id = buy.id; 
                        templog.sell_req_id = sell_q[item_id][i].id;
		        trade_data[trade_data_n++] = templog;
                }
            }
        }
        
        //if no seller found, insert in buy queue
        if(check == 0)
            sort_insert(buy);
        
        writeSocket(new_socket_fd, "SUCCESS\n");
    }



    else if(strcmp(message[2], "SELL")==0)
    {
        int userNum;
        int res = check_auth(message[0], message[1], &userNum);
        if(res != 1)
        {
            writeSocket( new_socket_fd, "FAIL\n Log in unsuccessful!\n") ;
            close(new_socket_fd);
            continue;
        }
        
        int i;
        int item_id = atoi( message[3] );
        int qty =  atoi( message[4] );
        int price_unit = atoi( message[5] );

        struct rqst sell;
        strcpy(sell.user, message[0]);
        sell.item_num = item_id;
        sell.qty = qty;
        sell.price = price_unit;
        sell.id = req_num++;
        sell.type = 'S';
        
        req_user[userNum][ num_of_request[ userNum ]++ ] = sell;
        
        while( sell.qty > 0 )
        {
	//if no buyer available, add to sell queue
            if(buy_start[item_id] == buy_end[item_id])          
            {
                sort_insert(sell);
                break;
            }
            int bestSell = buy_start[item_id];
             //check for each sell request pending
            for(i=buy_start[item_id]+1; i != buy_end[item_id]; i = (i+1)%500)      
            {
                if( (buy_q[item_id][i].price > buy_q[item_id][bestSell].price) || ( (buy_q[item_id][i].price == buy_q[item_id][bestSell].price) && (buy_q[item_id][i].id < buy_q[item_id][bestSell].id) ) )
                    bestSell = i;
            }
            if( buy_q[item_id][bestSell].price >= sell.price )             
            {
		//sell whole amount in request
                if( buy_q[item_id][bestSell].qty > sell.qty )               
                {
			  //update the buy queue qty
                    buy_q[item_id][bestSell].qty -= sell.qty;             
                   
                    struct userlog templog; 
			strcpy(templog.seller, message[0]); 
			strcpy(templog.buyer, buy_q[item_id][bestSell].user);
		        templog.item_num = item_id; 
                        templog.price = sell.price; 
			templog.qty = sell.qty; 
			templog.buy_req_id = buy_q[item_id][bestSell].id; 
                        templog.sell_req_id = sell.id; 
			trade_data[trade_data_n++] = templog;
                    
                    sell.qty = 0;
                    break;
                }

                else                                                        
                {
                    sell.qty -= buy_q[item_id][bestSell].qty;              
                    struct userlog templog; 
			strcpy(templog.seller, message[0]);
		        strcpy(templog.buyer, buy_q[item_id][bestSell].user);
			templog.item_num = item_id; 
	                templog.price = sell.price;
			templog.qty = buy_q[item_id][bestSell].qty; 
			templog.buy_req_id = buy_q[item_id][bestSell].id; 
                        templog.sell_req_id = sell.id; 
			trade_data[trade_data_n++] = templog;

                    //remove the entry form buy queue
                    for( i=bestSell; i!=buy_end[item_id]; i=(i+1)%500 )
                        buy_q[item_id][i] = buy_q[item_id][(i+1)%500];
                    buy_end[item_id] = (500+buy_end[item_id] - 1)%500;
                }
            }
            else                                                            
            {
                sort_insert(sell);                                           
                break;
            }
        }
        writeSocket(new_socket_fd, "SUCCESS\n");
    }
   
    else if(strcmp(message[2], "VIEW_ORDERS")==0)
    {
        int i;
        char message[500] = "\0";
        for(i=0; i<10; i++)
        {
            sprintf(message+strlen(message), "\n   Item_%d\n", i);
            strcpy(message+strlen(message), "   best sell: ");
            if( sell_start[i] != sell_end[i] )
            {
                sprintf(message+strlen(message), "quantity %d",sell_q[i][sell_start[i]].qty );
                strcpy(message+strlen(message), ", ");
                sprintf(message+strlen(message), "price %d", sell_q[i][sell_start[i]].price);
            }
            else
                strcpy(message+strlen(message), " ");
            strcpy(message+strlen(message), "\n");
              
            strcpy(message+strlen(message), "   best Buy:  ");
            if( buy_start[i] != buy_end[i] )
            {  
                sprintf(message+strlen(message), "quantity %d", buy_q[i][(500+buy_end[i]-1)%500].qty);
                strcpy(message+strlen(message), ",");
                sprintf(message+strlen(message), "price %d", buy_q[i][(500 + buy_end[i]-1)%500].price);
            }
            else
                strcpy(message+strlen(message), " ");
            strcpy(message+strlen(message), "\n");
        }
        writeSocket(new_socket_fd, "SUCCESS\n");
        writeSocket(new_socket_fd, message);
    }  
    else if(strcmp(message[2], "VIEW_TRADES")==0)
    {
        int userNum;
        int res = check_auth(message[0], message[1], &userNum);
        printf("%s: %d\n", message[0], num_of_request[userNum]);
        if(res != 1)
        {
            writeSocket( new_socket_fd, "FAIL\nLog in unsuccessful!\n") ;
            close(new_socket_fd);
            continue;
        }
        char sendmsg[500];
        writeSocket(new_socket_fd, "SUCCESS\n");
        int i, j;
        for(j=0; j<num_of_request[userNum]; j++)                                 
        {
            int getBuyID = req_user[userNum][j].id;                    
            for(i=0; i<trade_data_n; i++)                                      
            {
                if(trade_data[i].buy_req_id == getBuyID)                   
                {
                    sprintf(sendmsg, "\n BUYER: %s \n SELLER: %s \n ITEM: %d \n QUANTITY: %d \n PRICE: %d \n\n", trade_data[i].buyer, trade_data[i].seller, trade_data[i].item_num, trade_data[i].qty, trade_data[i].price);
                    writeSocket(new_socket_fd, sendmsg);
                }
                else if(trade_data[i].sell_req_id == getBuyID)              
                {
                    sprintf(sendmsg, "\n BUYER: %s \n SELLER: %s \n ITEM: %d \n QUANTITY: %d \n PRICE: %d \n\n", trade_data[i].buyer, trade_data[i].seller, trade_data[i].item_num, trade_data[i].qty, trade_data[i].price);
                    writeSocket(new_socket_fd, sendmsg);
                }
            }
        }
    }  
    close(new_socket_fd);
   }
   return 0;
}
