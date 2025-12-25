#include <stdio.h>
#include <string.h>

// looks like a classic BFS problem. We can present the state as bitmask and then present all
// operations as bitmasks too. the just BFS from initial state, taking each operation once in each
// step and seeing where it takes us. If we reach state 0, we found teh solution
int main()
{
	printf("Hello BFS (again)!\n");
	FILE *in_file = fopen("in", "r");
	char buffer[500];
	int n = 0, m = 0;
	while (fgets(buffer, 500, in_file) != NULL) {
		printf("%s\n", buffer);
        // TODO: replace strtok with strsep!!!
		char *token = strtok(buffer, " ");
		do {
			if (token[0] != '[' || token == NULL) {
				break;
			}
			printf("state: %s\n", token);

			token = strtok(NULL, " ");
		} while (token != NULL && token[0] == '[');

		do {
			if (token[0] != '(' || token == NULL) {
				break;
			}
			printf("operation: %s\n", token);

			token = strtok(NULL, " ");
		} while (token != NULL && token[0] == '(');

		do {
			if (token[0] != '{' || token == NULL) {
				break;
			}
			printf("rating: %s\n", token);

			token = strtok(NULL, " ");
		} while (token != NULL && token[0] == '{');
	}
}
