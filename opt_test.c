#include <stdio.h>
#include <getopt.h>

int main(int argc, char **argv)
{
	int opt;
	char *opt_string = "ab:c::d";
	if(argc < 2){
		printf("need arguments.");
		return -1;
	}
	while((opt = getopt(argc, argv, opt_string)) != -1){
		switch(opt){
			case 'a':
				printf("option = a, optind = %d, optarg = %s\n", optind, optarg);
				break;
			case 'b':
				printf("option = b, optind = %d, optarg = %s\n", optind, optarg);
				break;
			case 'c':
				printf("option = c, optind = %d, optarg = %s\n", optind, optarg);
				break;
			case 'd':
				printf("option = d, optind = %d, optarg = %s\n", optind, optarg);
				break;
			default:
				break;
		}
	}

	printf("argc = %d, optind = %d\n", argc, optind);
	return 0;
}
