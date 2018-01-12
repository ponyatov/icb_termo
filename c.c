#include "h.h"

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
	coap_address_t serv_addr;
	coap_address_init(&serv_addr);

}
