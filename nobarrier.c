/*!gcc {0} -I. -I../include/ -o out; ./out */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef snprintf
#define snprintf(buffer, size, ...) sprintf(buffer, __VA_ARGS__)
#endif

const char *possible_addresses[] = {
	// 8
	"192.168.8.100:24800",
	"192.168.8.101:24800",
	"192.168.8.102:24800",
	"192.168.8.103:24800",
	"192.168.8.104:24800",
	"192.168.8.105:24800",
	"192.168.8.106:24800",
	"192.168.8.107:24800",
	"192.168.8.108:24800",
	"192.168.8.109:24800",
	
	// 9
	"192.168.9.100:24800",
	"192.168.9.101:24800",
	"192.168.9.102:24800",
	"192.168.9.103:24800",
	"192.168.9.104:24800",
	"192.168.9.105:24800",
	"192.168.9.106:24800",
	"192.168.9.107:24800",
	"192.168.9.108:24800",
	"192.168.9.109:24800",
	
	// 10
	"192.168.10.100:24800",
	"192.168.10.101:24800",
	"192.168.10.102:24800",
	"192.168.10.103:24800",
	"192.168.10.104:24800",
	"192.168.10.105:24800",
	"192.168.10.106:24800",
	"192.168.10.107:24800",
	"192.168.10.108:24800",
	"192.168.10.109:24800"
};

unsigned barrier_server_is_running(const char *address)
{
	FILE *fp;
	char path[1035];
	char *concatenated = (char *) malloc(200);
	snprintf(concatenated, 200, "curl -I --silent --max-time 4 %s\0", address);
	printf("Checking '%s'... ", concatenated);
	clock_t begin = clock();
	fp = popen(concatenated, "r");
	if (fp == NULL) {
		printf("Failed to run command\n" );
		exit(1);
	}
	while (fgets(path, sizeof(path), fp) != NULL) {
		printf("%s", path);
	}
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	pclose(fp);
	printf("(%f) \n", time_spent);
	if (time_spent <= 2) {
		free(concatenated);
		return 1;
	}
	free(concatenated);
	return 0;
}

void check_possible_open_barrier_server(char *name)
{
	unsigned index;
	for(index = 0; index < 30; index++) {
		const char *address = possible_addresses[index];
		if (barrier_server_is_running(address) == 1) {
			char *concatenated = (char *) malloc(200);
			snprintf(concatenated, 200, "barrierc -f --name %s %s\0", name, address);
			printf("Barrier running at %s\n", address);
			system(concatenated);
			free(concatenated);
			return;
		}
	}
	check_possible_open_barrier_server(name);
}

int main(int argc, char **argv)
{
	check_possible_open_barrier_server(argv[1]);
	return EXIT_SUCCESS;
}