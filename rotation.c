/* 
rotation.c contains tools for encryption and decryption using a 
	rotation cipher
*/

// constants for ascii values of letters
static int a_idx = (int) 'a';
static int z_idx = (int) 'z';
static int A_idx = (int) 'A';
static int Z_idx = (int) 'Z';
static int case_offset = (int) 'a' - (int) 'A';
static int alphabet_length = (int) ('Z' - 'A' + 1);

/*
encryptRotation takes a pointer to a string and an integer value 
	representing a character offset, and passes the string through a 
	rotation cipher, using the given offset. The string is mutated to
	ciphertext.
encryptRotation: Int Char* -> Void
Requires: cleartext is null-terminated
Effects: cleartext is mutated to its own ciphertext
*/
void encryptRotation(int offset, char* cleartext) {
	while (*cleartext !=  0) {
		if ((*cleartext >= a_idx) && (*cleartext <= z_idx)) {
			*cleartext = *cleartext - case_offset;
		}
		if (*cleartext >= A_idx && *cleartext <= Z_idx) {
			*cleartext = A_idx + (*cleartext - A_idx + offset) % alphabet_length;
		}
		cleartext++;
	}
}
	
/*
decryptRotation takes in an integer representing a character offset, and
	a string of ciphertext, produced using a rotation cipher with the
	given offset. It mutates the string to the cleartext version of
	itself.
decryptRotation: Int Char* -> Void
Requires: ciphertext is null-terminated
Effects: ciphertext is mutated back to cleartext

*/
void decryptRotation(int offset, char* ciphertext) {
	encryptRotation(alphabet_length - offset, ciphertext);
}
