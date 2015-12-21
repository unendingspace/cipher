#include <stdio.h>
#include <stdlib.h>
#include "rotation.c"
#include "frequency.c"

int stringLength(char* string) {
	int length = 0;
	while (*string) {
		string++;
		length++;
	}
	return length;
}
	
char* cloneString(char* string) {
	char* clone = malloc(sizeof(char) * (stringLength(string) + 1));
	char* out = clone;
	while (*string) {
		*clone = *string;
		clone++;
		string++;
	}
	*clone = 0;
	return out;
}

void deleteClone(char* clone) {
	free(clone);
}

int main() {
	char* string = "This is a test message";
	char* message = cloneString(string);
	printf("Cleartext: %s\n", message);
	encryptRotation(13, message);
	printf("Ciphertext: %s\n", message);
	decryptRotation(13, message);
	printf("Transmission: %s\n", message);
	deleteClone(message);
}
