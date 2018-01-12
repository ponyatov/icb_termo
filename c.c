#include "h.h"

static void
hello_handler(coap_context_t *ctx, struct coap_resource_t *resource,
              const coap_endpoint_t *local_interface, coap_address_t *peer,
              coap_pdu_t *request, str *token, coap_pdu_t *response)
{
	response->hdr->code = COAP_RESPONSE_CODE(205);
	unsigned char buf[3];
	coap_add_option(response, COAP_OPTION_CONTENT_TYPE,\
			coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf); // text
	unsigned char response_data[] = "Hello World!";
	coap_add_data(response,strlen((char*)response_data),response_data);
}

int main(int argc, char *argv[]) {

	// cmd.line arguments

	for (int i=0;i<argc;i++)
		printf("argv[%i]=%s\n",i,argv[1]);
	printf("\n");

	// reading [lm]sensors

	sensors_init(NULL);

	printf("sensors_version=%s\n\n",libsensors_version);

	sensors_chip_name const * cn;
	int c = 0;
	while ((cn = sensors_get_detected_chips(0, &c)) != 0) {
		printf("chip: %s %s\n",cn->prefix,cn->path);

		sensors_feature const *feat;
		int f = 0;
		while ((feat = sensors_get_features(cn, &f)) != 0) {
			#ifdef LOG
			printf("\t%i:%s/%s\n",f,cn->prefix,feat->name);
			#endif // LOG

			sensors_subfeature const *subf; int s=0;
			while ((subf = sensors_get_all_subfeatures(cn, feat, &s)) != 0) {
				double val=0;
				if (subf->flags & SENSORS_MODE_R) { // readable
					int rc = sensors_get_value(cn, subf->number, &val);
					if (rc<0)
						fprintf(stderr,"error:%i\n",rc);
					#ifdef LOG
					else
						printf("\t\t%s=%.1f\n",subf->name,val);
					#endif // LOG
				}
			}
		}
	}

	sensors_cleanup();

	// CoAP server initialize
	// // server address
	coap_address_t serv_addr;
	coap_address_init(&serv_addr);
	serv_addr.addr.sin.sin_family		= AF_INET; // IP
	serv_addr.addr.sin.sin_addr.s_addr	= htonl(INADDR_LOOPBACK);// localhost only
	serv_addr.addr.sin.sin_port			= htons(COAP_PORT);
	// // CoAP context
	coap_context_t* ctx;
	ctx = coap_new_context(&serv_addr); assert (ctx);// open context
	// // hello resource
	coap_resource_t* res_hello;
	res_hello = coap_resource_init((unsigned char*)"hello", 5, 0);
	coap_register_handler(res_hello,COAP_REQUEST_GET,hello_handler);
	coap_add_resource(ctx, res_hello);				// register resource
	// CoAP server loop
//	while (1) {
		fd_set readfds; FD_ZERO(&readfds); // feed
		FD_SET(ctx->sockfd, &readfds);
		int result = select( FD_SETSIZE, &readfds, 0, 0, NULL ); // wait/read
		assert(result>=0);
		if (FD_ISSET(ctx->sockfd, &readfds)) coap_read(ctx);
//	}
}
