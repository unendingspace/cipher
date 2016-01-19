/*
frequency.c contains tools for letter frequency analysis
*/

/*
letterCounter is a structure for keeping track of how many times a
	particular character is encountered in some array. It holds the 
	letter which is being counted, and the current total of times it has
	been seen
*/
struct letterCounter {
	char letter;
	int count;
};

/*
frequencyData is a wrapper structure for the letterCounter structure.
	It holds an array of letterCounter structures, as well as the total
	number of alphabetic characters counted, and the total number of 
	non-alpabetic characters counted
*/
struct frequencyData {
	struct letterCounter* data;
	int total;
	int non_alpha;
};

/*
printLetterFrequency takes in a frequencyData structure, and prints the
	number of occurrences of each letter to the screen.
printLetterFrequency: frequencyData* -> Void
Effects: Prints letter frequency data to the screen
*/
void printLetterFrequency(struct frequencyData* dataset) {
	struct letterCounter* data = (*dataset).data;
	for (int i = 0; i < alphabet_length; i++) {
		printf("%c: %i\n", (*data).letter, (*data).count);
		data++;
	}
}

/*
genBar takes in an integer and produces the pound symbol ('#') a number 
	of times equal to the value of the integer.
genBar: Int -> Void
Effect: prints a line of #s to the screen
*/
static void genBar(int num) {
	while (num > 0) {
		printf("#");
		num--;
	}
	printf("\n");
}

/*
graphLetterFrequency takes in a pointer to a frequencyData structure,
	and graphs the data contained in it.
graphLetterFrequency: frequencyData* -> Void
Effect: prints frequency data to screen
*/
void graphLetterFrequency(struct frequencyData* dataset) {
	
	printf("\nApproximately %i%% percent of characters were alphabetic.\nThey were distributed as follows:\n\n", 100 * (*dataset).total /
																												((*dataset).total + (*dataset).non_alpha));
	struct letterCounter* data = (*dataset).data;
	for (int i = A_idx; i <= Z_idx; i++) {
		printf("%c: ", i);
		genBar(200 * ((*dataset).data[i - A_idx]).count / (*dataset).total);
	}
	printf("\n");
}

/*
deleteFrequencyData takes in a pointer to a frequencyData structure, and
	frees the allocated memory within the structure, as well an that
	holding the structure itself
deleteFrequencyData: frequencyData* -> Void
Effects: frees all memory associated with the frequencyData structure
*/
void deleteFrequencyData(struct frequencyData* dataset) {
	free((*dataset).data);
	free(dataset);
}

/*
genLetterFrequency takes in a string and analyzes it, giving back a
	frequencyData structure
genLetterFrequency: Char* -> frequencyData*
Requires: ciphertext is null-terminated
*/
struct frequencyData* getLetterFrequency(char* ciphertext) {
	int total = 0;
	int non_alpha = 0;
	struct letterCounter* data = malloc(sizeof(struct letterCounter) * 26);
	struct frequencyData* output = malloc(sizeof(struct frequencyData));
	for (int i = A_idx; i <= Z_idx; i++) {
		data[i - A_idx].letter = (char) i;
	}
	while (*ciphertext) {
		if (a_idx <= *ciphertext && z_idx >= *ciphertext) {
			((data[((int) *ciphertext) - a_idx]).count)++;
			total++;
			
		}
		else if (A_idx <= *ciphertext && Z_idx >= *ciphertext) {
			((data[((int) *ciphertext) - A_idx]).count)++;
			total++;
		}
		else {
			non_alpha++;
		}
		ciphertext++;
	}
	(*output).data = data;
	(*output).total = total;
	(*output).non_alpha = non_alpha;
	return output;
}
