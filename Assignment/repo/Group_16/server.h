struct rqst
{
	int id;
    	char user[10];
	char type; 
    	int item_num;
    	int qty;
    	int price;
    	    
    
};

struct userlog      
{
    char buyer[10];
    char seller[10];
    int buy_req_id;
    int sell_req_id;
    int item_num;
    int price;
    int qty;
   
};

void sort_insert(struct rqst);   
int trade_data_n = 0;
int num_of_request[5] = {0};
int req_num=0;

struct rqst req_user[5][500];       
struct userlog trade_data[5000];              
struct rqst buy_q[10][500];
struct rqst sell_q[10][500];

int buy_start[10]={0}, buy_end[10]={0}, sell_start[10]={0}, sell_end[10]={0};

    


