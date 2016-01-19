/* 
fileio.c contains tools for taking input from and outputting to 
	text files
*/

#include <stdio.h>
#include <stdlib.h>

static FILE* input;
static char* input_path;
static FILE* output;
static char* output_path;

static char* read = "r";
static char* write = "w";
static char newline = '\n';
static char tab = '\t';
static char space = ' ';
static int line_len = 65;

/*
testExists takes in a path to a file, and returns 1 if the file exists
	on the filesystem, and 0 otherwise. 
testExists: Char* -> Int
Requires: path is null-terminated
*/
static int testExists(char* path) {
	FILE* exists = fopen(path, read);
	if (exists) {
		fclose(exists);
		return 1;
	}
	else {
		return 0;
	}
}

/*
clearStdin takes no arguments, and clears the STDIN buffer of all
	characters up to either a newline or EOF
clearStdin: Void -> Void
Requires: STDIN buffer is not empty
Effects: STDIN buffer is now empty
*/
static void clearStdin() {
	int garbage;
	while (garbage != newline && garbage != EOF) {
		garbage = fgetc(stdin);
	}
}

/*
ynPrompt takes in a prompt to give the user, and allows the user to
	answer the prompt with 'yes' ('y') or 'no' ('n'). It prompts the user
	repeatedly until one of these answers is given, then returns 1 to
	indicate 'yes', and 0 for 'no'
ynPrompt: Char* -> Void
Requires: prompt_text is null-terminated
Effects: Prints one, or possibly more, prompts to the screen, and takes
	     user input
*/
static int ynPrompt(char* prompt_text) {
	char* error_prompt = "\nPlease enter either 'y' or 'n'\n\n";
	printf("%s (y/n) ", prompt_text);
	char answer = NULL;
	scanf("%c", &answer);
	if (answer == newline) {
		printf(error_prompt);
		return ynPrompt(prompt_text);
	}
	else if (answer == 'y' || answer == 'Y') {
		scanf("%c", &answer);
		if (answer == newline || answer == EOF) {
			printf("%c", newline);
			return 1;
		}
		else {
			printf(error_prompt);
			clearStdin();
			return ynPrompt(prompt_text);
		}
	}
	else if (answer == 'n' || answer == 'N') {
		scanf("%c", &answer);
		if (answer == newline || answer == EOF) {
			printf("%c", newline);
			return 0;
		}
		else {
			printf(error_prompt);
			clearStdin();
			return ynPrompt(prompt_text);
		}
	}
	else {
		clearStdin();
		printf(error_prompt);
		return ynPrompt(prompt_text);
	}
}

/*
setInput takes a path to a file, and attempts to open it for reading. It
	returns 1 if it is successful, and 0 if it fails.
setInput: Char* -> Int
Requires: path is null-terminated
Effects: input and input_path are mutated to match the selected file
*/
int setInput(char* path) {
	FILE* new = fopen(path, read);
	if (new) {
		if (input) {
			fclose(input);
		}
		input = new;
		input_path = path;
		return 1;
	}
	else {
		return 0;
	}
}

/*
setOutput takes in a path to a file, and attempts to set it as the
	output file. If the file already exists, it prompts the user for
	confirmation before opening it. If it is successful, it return 1, 
	returning 0 if it fails
setOutput: Char* -> Int
Requires: path is null-terminated
Effects: output and output-path are mutated
*/
int setOutput(char* path) {
	char* exists_message = "\nThe file you have selected already exists. Writing to it will \ncause any data currently stored there to be erased.\n";
	char* continue_prompt = "Do you wish to continue?";
	if (testExists(path)) {
		printf(exists_message);
		int cont = ynPrompt(continue_prompt);
		if (!(cont)) {
			return 0;
		}
	}		
	FILE* new = fopen(path, write);
	if (new) {
		if (output) {
			fclose(output);
		}
		output = new;
		output_path = path;
		return 1;
	}
	else {
		return 0;
	}
}

/*
closeInput takes no arguments, and closes any open input file
closeInput: Void -> Void
Effects: file pointed to by input is closed, input is set to null
*/
void closeInput() {
	if (input) {
		fclose(input);
		input = NULL;
	}
}

/*
closeOutput takes no arguments, and closes any open output file
closeOutput: Void -> Void
Effects: file pointed to by output is closed, output is set to null
*/
void closeOutput() {
	if (output) {
		fclose(output);
		output = NULL;
	}
}

/*
getDataHolder takes no arguments, and returns a character array large
	enough to hold one line plus one letter, and empty except for the
	last slot, which holds the null terminator
getDataHolder: Void -> Char*
Effects: memory is allocated from heap, and must be freed manually later
*/
static char* getDataHolder() {
	char* holder = malloc(sizeof(char) * (line_len + 1));
	holder[line_len] = 0;
	return holder;
}

/*
deleteDataHolder takes in a character array which has been allocated
	from the heap, and frees its memory
deleteDataHolder: Char* -> Void
Requires: data_holder points to memory allocated on heap
Effects: memory pointed to by data_holder is freed
*/
static void deleteDataHolder(char* data_holder) {
	free(data_holder);
	data_holder = NULL;
}

/*
readLine takes in an array to read characters into, and reads in a 
	specified number of characters from the input file. It returns 0 if
	it has reached EOF, and 1 otherwise
readLine: Char* -> Int
Requires: read_into has enough spaces for one line of characters
		  input file is not null
Effects: read_into is mutated
		 position in input file is advanced
*/
static int readLine(char* read_into) {
	int i = 0;
	char next = 0;
	while (i < line_len) {
		if (fscanf(input, "%c", &next) < 0) {
			*read_into = 0;
			return 0;
		}
		else if (next == newline || next == tab) {
			*read_into = space;
			read_into++;
			i++;
		}
		else {
			*read_into = next;
			read_into++;
			i++;
		}
	}
	return 1;
}

/*
writeLine takes an array to write from, and writes characters from the	
	array to the output file. It returns 1 if a full line of characters
	was written, and 0 otherwise
writeLine: Char* -> Int
Requires: output file is not null
Effects: characters from write_from are written to output file
		 position in output file is advanced
*/
static int writeLine(char* write_from) {
	int i = fprintf(output, "%s", write_from);
	if (i == line_len) {
		fprintf(output, "\n");
	}
	return i == line_len;
}

/*
transmitData reads characters from the input file, line by line, puts
	them through the cipher function given as an argument, and writes
	the ciphertext to the output file
transmitData: (Char* -> Void)* -> Void
Requires: cipher is non-null
		  input file is non-null
		  output file is non-null
Effects: Writes cleartext from input file to ciphertext from output file
*/
void transmitData(void (*cipher)(char*)) {
	char* data_holder = getDataHolder();
	int cont = 1;
	while (cont) {
		cont = readLine(data_holder);
		(*cipher)(data_holder);
		writeLine(data_holder);
	}
	deleteDataHolder(data_holder);
}

