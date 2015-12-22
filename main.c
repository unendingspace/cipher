#include <stdio.h>
#include <stdlib.h>
#include "rotation.c"
#include "frequency.c"
#include "fileio.c"

int offset = 7;

void encryptFixedRotation(char* string) {
	return encryptRotation(offset, string);
}

void decryptFixedRotation(char* string) {
	decryptRotation(offset, string);
}

int main() {
	char* clear_path = "testingclear";
	char* cipher_path = "testingcipher";
	char* transmit_path = "testingtransmission";
	
	setInput(clear_path);
	setOutput(cipher_path);
	
	transmitData(&encryptFixedRotation);
	
	closeInput();
	closeOutput();
	
	setInput(cipher_path);
	setOutput(transmit_path);
	
	transmitData(&decryptFixedRotation);
	
	closeInput();
	closeOutput();
}
