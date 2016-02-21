/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <string.h>

typedef struct {
    /* the round robin data must be first */
    ngx_http_upstream_rr_peer_data_t   rrp;
    ngx_str_t 				uri;    
    char**				ip_val_list;
  //  char**				function_val_list;
    int					ds_max_count;
    int 				index;
} ngx_http_upstream_prum_peer_data_t;


static ngx_int_t ngx_http_upstream_init_prum_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_get_prum_peer(ngx_peer_connection_t *pc,
    void *data);
static char *ngx_http_upstream_prum(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


static ngx_command_t  ngx_http_upstream_prum_commands[] = {

    { ngx_string("prum"),
      NGX_HTTP_UPS_CONF|NGX_CONF_NOARGS,
      ngx_http_upstream_prum,
      0,
      0,
      NULL },

      ngx_null_command
};


int resp_time_arr[50];
int resp_time_sum;
int resp_time_count;

static ngx_http_module_t  ngx_http_upstream_prum_module_ctx = {
    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_upstream_prum_module = {
    NGX_MODULE_V1,
    &ngx_http_upstream_prum_module_ctx, /* module context */
    ngx_http_upstream_prum_commands,    /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_int_t
ngx_http_upstream_init_prum(ngx_conf_t *cf, ngx_http_upstream_srv_conf_t *us)
{
    if (ngx_http_upstream_init_round_robin(cf, us) != NGX_OK) {
        return NGX_ERROR;
    }

    us->peer.init = ngx_http_upstream_init_prum_peer;

    return NGX_OK;
}


static ngx_int_t
ngx_http_upstream_init_prum_peer(ngx_http_request_t *r,
		ngx_http_upstream_srv_conf_t *us)
{
	ngx_http_upstream_prum_peer_data_t  *iphp;

	iphp = ngx_palloc(r->pool, sizeof(ngx_http_upstream_prum_peer_data_t));
	if (iphp == NULL) {
		return NGX_ERROR;
	}

	r->upstream->peer.data = &iphp->rrp;

	if (ngx_http_upstream_init_round_robin_peer(r, us) != NGX_OK) {
		return NGX_ERROR;
	}

	iphp->uri = r->uri;

	FILE * fp;
	char * line = NULL;
	fp = fopen("/home/prudhvi/abc.txt", "r");
	size_t len = 0;
	ssize_t read;
	char parse_str[1000];
	char parse_str2[1000];
	char *parse;
	char *parse2;
	int count_ds = 0; 
	int counter1 = 0;
	int min_time_peer = 1000000;
	int counter2 = 0;
	char str_uri[1000];
	int index_min_time = 0;

	printf("******************* \n");
	printf("Persistant DS check value -> %d \n", (*us).check);

	if(((*us).check) == 0)
	{
		while ((read = getline(&line, &len, fp)) != -1) 
		{
                     
	//		printf("Line from file -> %s \n", line);
			strcpy(parse_str, line);
			strcpy(parse_str2, line);
			parse = strtok (parse_str, " ");
	//		printf("Function from file -> %s \n", parse);

			parse2 = strtok(parse_str2, " ");
			parse2 = strtok(NULL, " ");
	//		printf("IP from file -> %s \n", parse2);

			us->function_name[count_ds] = malloc(50);
			us->ip_val[count_ds] = malloc(50);    

			strcpy((*us).function_name[count_ds],parse);
			strcpy((*us).ip_val[count_ds],parse2);

			count_ds++;	

		}
		(*us).max_count = count_ds;
		
		for(counter1=0; counter1<50; counter1++)
		{
			resp_time_arr[counter1] = 0;
		}

	}

	fclose(fp);

	/*
		code to update recent_time
	*/
/*
	if((*us).check != 0)
	{
		printf("Value of global variable in our prum module -> %s \n", prev_up_resp);
		char *tmp_parser;
		char tmp_prev_up[500];
		char tmp_index[10];
		char tmp_time[50];
		int index;
		int up_serv_time;
		strcpy(tmp_prev_up, prev_up_resp);
		tmp_parser = strtok (tmp_prev_up, " ");
		strcpy(tmp_index, tmp_parser);
		index = atoi(tmp_index);
		printf("Index of Upstream time array to be updated -> %d \n", index);
		tmp_parser = strtok(NULL, " ");
		strcpy(tmp_time, tmp_parser);
		up_serv_time = atoi(tmp_time);
		printf("Time of Upstream time to be updated -> %d \n", up_serv_time);
		
		(*us).recent_time[index] = up_serv_time;	
	}
*/

	/*
		code to parse uri for R function 
	*/

	strcpy(str_uri, iphp->uri.data);
  //      printf("%s \n", str_uri);
        char *tmp_pointer = str_uri;

	char *pch;
	char *tmp_rfunc;
        pch = strtok (str_uri,"/");
        pch = strtok(NULL, "/");
	int function_match = 0;
	
	if(strcmp(pch, "user")==0)
        {
                int count =0;
                while (count < 5)
                {
                        pch = strtok (NULL, "/");
                        count++;
                }

                tmp_rfunc = strtok(pch, " ");
//		printf("R function from URI -> %s \n", tmp_rfunc);
	}

	/*
		code to select a peer
	*/
/*
	   int aa = 0;
           printf("---------------- \n");
           for(aa=0; aa<(*us).max_count; aa++)
           {
           printf("Data structure values in get peer function -> %s and %s \n", (*us).function_name[aa], (*us).ip_val[aa]);
           }
           printf("---------------- \n");
*/
	for(counter2 = 0; counter2<(*us).max_count; counter2++)
	{
		if(strcmp((*us).function_name[counter2], tmp_rfunc) == 0)
		{
			function_match = 1;
/*			printf("******** \n ");
			printf("counter number = %d \n", counter2);
			printf("(*us).function_name[counter2] value = %s \n", (*us).function_name[counter2]);
			printf("tmp_rfunc = %s \n", tmp_rfunc);
			printf("******** \n ");
			printf("Function Match \n");
*/
//			printf("tmp_rfunc = %s \n", tmp_rfunc);
			printf("Function Match \n");
			if(min_time_peer > resp_time_arr[counter2])
			{
				min_time_peer = resp_time_arr[counter2];
				index_min_time = counter2;
//				printf("counter number again = %d \n", counter2);
//				printf("index_min_time = %d \n", index_min_time);	
			}
		}
	}
	
	
	if(function_match == 0)
	{
		printf("No Function Match. Sending it to default server. \n");
		(*iphp).index = 8;		//Default server
	}
	else
	{
		printf("Min time is taken by server with index %d. It is %d milliseconds \n ", index_min_time, min_time_peer);	
		(*iphp).index = index_min_time;
	}
	
	(*us).check++;

	(*r).index = (*iphp).index;
 	
	iphp->ip_val_list = us->ip_val;
//	iphp->function_val_list = us->function_name;
	(*iphp).ds_max_count = (*us).max_count;
	
	r->upstream->peer.get = ngx_http_upstream_get_prum_peer;

	return NGX_OK;
}


static ngx_int_t
ngx_http_upstream_get_prum_peer(ngx_peer_connection_t *pc, void *data)
{
	ngx_http_upstream_prum_peer_data_t  *iphp = data;
	ngx_http_upstream_rr_peer_t  *peer;
	
	int loop_count = 0;
	int num_serv;
	struct sockaddr_in *sin;  
	char *ip;
	int ip_index;

	peer = iphp->rrp.peers->peer;
	num_serv = iphp->rrp.peers->number;				
	
	/*
		Findind the peer based on index
	*/

	ip_index = (*iphp).index;

	for(loop_count = 0; loop_count<num_serv; loop_count++)
	{
		sin = malloc(sizeof(struct sockaddr_in));
		sin = (struct sockaddr_in *) peer->sockaddr;
		ip = malloc(150);
		ip = inet_ntoa(sin->sin_addr);
//		printf("IP from Peer list -> %s \n", ip);

		//Need to make this generic. Currently works only for our VM scenario.

		if(strncmp(ip, (*iphp).ip_val_list[ip_index], 13) == 0)
		{
			printf("IP Match \n");
			break;
		}
		if(loop_count < num_serv-1)
			peer = peer->next;
		else
			break;

	}

	if(loop_count == num_serv)
	{	
		printf("No IP match between DS and Peer list. Sending to default server. \n");

		peer = iphp->rrp.peers->peer;
		peer = peer->next;
	}

	printf("*********************** \n");

	iphp->rrp.current = peer;
	pc->sockaddr = peer->sockaddr;
	pc->socklen = peer->socklen;
	pc->name = &peer->name;

	return NGX_OK;
}


static char *
ngx_http_upstream_prum(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_upstream_srv_conf_t  *uscf;

    uscf = ngx_http_conf_get_module_srv_conf(cf, ngx_http_upstream_module);

    if (uscf->peer.init_upstream) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "load balancing method redefined");
    }

    uscf->peer.init_upstream = ngx_http_upstream_init_prum;

    uscf->flags = NGX_HTTP_UPSTREAM_CREATE
                  |NGX_HTTP_UPSTREAM_WEIGHT
                  |NGX_HTTP_UPSTREAM_MAX_FAILS
                  |NGX_HTTP_UPSTREAM_FAIL_TIMEOUT
                  |NGX_HTTP_UPSTREAM_DOWN;

    return NGX_CONF_OK;
}
