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
    char**				function_val_list;
    int					ds_max_count;
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
	//  r->upstream->peer.get = ngx_http_upstream_get_prum_peer;
	/*
	   printf("Persistant int -> %d \n" ,(*us).num);
	   (*us).num++;
	 */

	FILE * fp;
	char * line = NULL;
	fp = fopen("/home/pru/abc.txt", "r");
	size_t len = 0;
	ssize_t read;
	char parse_str[1000];
	char parse_str2[1000];
	char *parse;
	char *parse2;
	int count_ds = 0; 

	if(((*us).check) == 0)
	{
		while ((read = getline(&line, &len, fp)) != -1) 
		{

			printf("Line from file -> %s \n", line);
			strcpy(parse_str, line);
			strcpy(parse_str2, line);
			parse = strtok (parse_str, " ");
			printf("Function from file -> %s \n", parse);

			parse2 = strtok(parse_str2, " ");
			parse2 = strtok(NULL, " ");
			printf("IP from file -> %s \n", parse2);

			us->function_name[count_ds] = malloc(50);
			us->ip_val[count_ds] = malloc(50);    

			strcpy((*us).function_name[count_ds],parse);
			strcpy((*us).ip_val[count_ds],parse2);

			count_ds++;	

		}
		(*us).max_count = count_ds;

	}
	(*us).check++;
	printf("Value of check: %d \n",  (*us).check);
	fclose(fp);
	
	iphp->ip_val_list = us->ip_val;
	iphp->function_val_list = us->function_name;
	(*iphp).ds_max_count = (*us).max_count;

	/*
	   int aa = 0;
	   printf("---------------- \n");

	   for(aa=0; aa<(*us).max_count; aa++)
	   {
	   printf("Data structure values -> %s and %s \n", (*us).function_name[aa], (*us).ip_val[aa]);
	   }

	   printf("---------------- \n");
	 */


	r->upstream->peer.get = ngx_http_upstream_get_prum_peer;

	return NGX_OK;
}


static ngx_int_t
ngx_http_upstream_get_prum_peer(ngx_peer_connection_t *pc, void *data)
{
    ngx_http_upstream_prum_peer_data_t  *iphp = data;

    ngx_http_upstream_rr_peer_t  *peer;

    char tmp_str[1000];
    strcpy(tmp_str, iphp->uri.data);
    char *tmp_pointer = tmp_str;

/*
	   int aa = 0;
           printf("---------------- \n");

           for(aa=0; aa<(*iphp).ds_max_count; aa++)
           {
           printf("Data structure values in get peer function -> %s and %s \n", (*iphp).function_val_list[aa], (*iphp).ip_val_list[aa]);
           }

           printf("---------------- \n");
	
*/
 
    printf("******************* \n");    

//    printf("str_temp -> %s \n", tmp_str); 
/*
    while (c = *tmp_pointer++)
        hash = ((hash << 5) + hash) + c;

    printf("Hash value %u \n", hash);
   
    int n = hash%2;

    printf("Sending request to Server %d \n", n + 1);

    printf("Number of servers available %u \n", iphp->rrp.peers->number);	
*/

        char *pch;
        pch = strtok (tmp_str,"/");
	pch = strtok(NULL, "/");
	int n;	
	int num_serv;	
	int index;
	int loop_count =0;
	struct sockaddr_in *sin;  
	char *ip;
	peer = iphp->rrp.peers->peer;
	peer = peer->next;
	if(strcmp(pch, "user")==0)
	{
		int count =0;
		while (count < 5)
		{
			pch = strtok (NULL, "/");
			count++;
		}

		char *tmp;
		tmp = strtok(pch, " ");

		for(index = 0; index<(*iphp).ds_max_count; index++)
		{
			printf("Function from Nginx URI -> %s\n", tmp);
			printf("Function from DS -> %s \n", (*iphp).function_val_list[index]);
			if(strcmp((*iphp).function_val_list[index], tmp) == 0)
			{
				break;
			}
		}		

		if(index == (*iphp).ds_max_count)
		{	
			printf("No Function Match \n");
			
		}
		if(index < (*iphp).ds_max_count )
		{
			printf("Function Match \n");	
			peer = iphp->rrp.peers->peer;
			printf("IP from DS -> %s \n", (*iphp).ip_val_list[index]);
			num_serv = iphp->rrp.peers->number;				
			for(loop_count = 0; loop_count<num_serv; loop_count++)
			{
				//			printf("Inside Loop \n");
				sin = malloc(sizeof(struct sockaddr_in));
				sin = (struct sockaddr_in *) peer->sockaddr;
				ip = malloc(150);
				ip = inet_ntoa(sin->sin_addr);
				printf("IP from Peer list -> %s \n", ip);
				//			printf("Length of string from Peer -> %d \n", strlen(ip));

				if(strncmp(ip, (*iphp).ip_val_list[index], 8) == 0)
				{
					printf("IP Match \n");
					break;
				}
				if(loop_count < num_serv-1)
					peer = peer->next;
				else
					break;
			}
			//		printf("Loop exited \n");

			if(loop_count == num_serv)
				printf("Peer not found! \n");
		}

	}
	else
	{
		printf("Non R Function URI \n");
		if(!peer)
		{
			peer = iphp->rrp.peers->peer;
			peer = peer->next;
		}
	}	
	iphp->rrp.current = peer;
	pc->sockaddr = peer->sockaddr;
	pc->socklen = peer->socklen;
	pc->name = &peer->name;

	return NGX_OK;


	// 		printf("Number of servers available %u \n", iphp->rrp.peers->number); 
  /*
    		n = 0;

    		if(strcmp(tmp, "server1") == 0)
			n = 0;
   		else
			n = 1;
    
     }
	else
		n = 1;
    
    printf("Sending request to Server %d \n", n + 1);
*/
//    printf("******************* \n");

//    peer = iphp->rrp.peers->peer;

/* 
    int h = 0;   
 
    for(h=0; h<n; h++)
    {
	peer = peer->next;    
    }
	 
    iphp->rrp.current = peer;

    pc->sockaddr = peer->sockaddr;
    pc->socklen = peer->socklen;
    pc->name = &peer->name;


	struct sockaddr_in *sin = (struct sockaddr_in *) peer->sockaddr;
	char *ip = inet_ntoa(sin->sin_addr);

    printf("IP -> %s \n", ip);
    if(strcmp(ip, "10.0.2.6")==0)
	{
	printf("True \n");
	} 
printf("******************* \n");
*/
//	return NGX_OK;
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
