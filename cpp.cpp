#include "hpp.hpp"

int main(int argc, char *argv[]) {

	// cmd.line arguments

	for (auto i=0;i<argc;i++)
		cerr << "argv[" << i << "]=" << argv[i] << endl;
	cerr << endl;

	// reading [lm]sensors

	sensors_init(NULL);

	cerr << "sensors_version=" << libsensors_version << endl << endl;

	sensors_chip_name const * cn;
	int c = 0;
	while ((cn = sensors_get_detected_chips(0, &c)) != 0) {
		cerr << "chip: " << cn->prefix << " " << cn->path << endl;

		sensors_feature const *feat;
		int f = 0;
		while ((feat = sensors_get_features(cn, &f)) != 0) {
			cerr << '\t'  << f << ':' << cn->prefix << '/' << feat->name << endl;

			sensors_subfeature const *subf; int s=0;
			while ((subf = sensors_get_all_subfeatures(cn, feat, &s)) != 0) {
				double val=0;
				if (subf->flags & SENSORS_MODE_R) { // readable
					int rc = sensors_get_value(cn, subf->number, &val);
					if (rc<0)
						cerr << "error:" << rc << endl;
					else
						cerr << "\t\t" << subf->name << '=' << val << endl;
				}
			}
		}
	}

	sensors_cleanup();

}
