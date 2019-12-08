#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <string.h>
#include <sys/wait.h> 
#include <stdlib.h>

#define HEX_IN "hex_in.tmp"
#define HEX_OUT "hex_out.log"
#define HEPT_IN "hept_in.tmp"
#define HEPT_OUT "hept_out.log"
#define OCT_IN "oct_in.tmp"
#define OCT_OUT "oct_out.log"

void redirect_command(int polygon_type, long long instruction, int fd) {
	bool new_polygon = 1 & (instruction >> 1);
	long long command_mask = 0b11111111000001;

	//new polygon and the same type as what recieved
	if (new_polygon && polygon_type == ((instruction >> 2) & 15)) {
		dprintf(fd, "%llx\n", instruction);
	}

	//execute operations for this type or execute operations for all types or this is the last operation (need to update all files)
	else if ((instruction >> 10) == 15 || polygon_type == (instruction >> 10) || instruction & 1) {
		dprintf(fd, "%llx\n", (instruction & command_mask));
	}
}

void redirect_input(long long instruction, bool * keep_going, int files_fd[]) {
	*keep_going = !(1 & instruction);
	int polygon_type_mask = 15;
	int i;
	bool new_polygon = 1 & (instruction >> 1);
	long long vertices;
	int polygon_type;

	//check for each command if realted for each type
	for (i = 0; i < 3; i++) {
		redirect_command(6 + i, instruction, files_fd[i]);
	}

	//get new polygon vertices
	if (new_polygon) {
		polygon_type = (instruction >> 2) & polygon_type_mask;
		
		for (i = 0; i < 2; i++) {
			scanf("%llx", &vertices);
			dprintf(files_fd[polygon_type - 6], "%llx\n", vertices);
		}
	}
}

void get_input(int files_fd[]) {
	long long instruction;
	bool keep_going;

	while (1) {
		scanf("%llx", &instruction);
		redirect_input(instruction, &keep_going, files_fd);

		if (!keep_going) {
			break;
		}
	}
}

void create_files(int input_fds[], int output_fds[]) {
	int i;
	char * input_files_name[] = { HEX_IN, HEPT_IN, OCT_IN };
	char * output_files_name[] = { HEX_OUT, HEPT_OUT, OCT_OUT };

	for (i = 0; i < 3; i++) {
		input_fds[i] = open(input_files_name[i], O_CREAT | O_RDWR);
		output_fds[i] = open(output_files_name[i], O_CREAT | O_RDWR);

		if (input_fds[i] < 0) {
			perror(strcat("Error to open input file: \n", input_files_name[i]));
		}

		if (output_fds[i] < 0) {
			perror(strcat("Error to open output file : \n", output_files_name[i]));
		}
	}
}

void set_cursors_to_start(int files_fd[]) {
	int i;

	for (i = 0; i < 3; i++) {
		lseek(files_fd[i], 0, SEEK_SET);
	}
}

void run_childs(int input_fds[], int output_fds[]) {
	int i, pid, child_status;
	char* my_argv[2];
	int child_pids[3];
	char * type;
	char prog_name[20] = "ex2_q1_helper";
	my_argv[0] = "ex2_q1_helper"; 
	my_argv[1] = NULL;


	for (i = 0; i < 3; i++) {
		if ((pid = fork()) == 0) {
			dup2(input_fds[i], STDIN_FILENO);
			dup2(output_fds[i], STDOUT_FILENO);
			
			close(input_fds[i]);
			close(output_fds[i]);
			execve(prog_name, my_argv, NULL);

			perror("ERROR:"); //error details
			fprintf(stderr, "*** ERROR: *** EXEC of %s FAILED\n", prog_name);
			exit(1);
		}

		child_pids[i] = pid;
	}

	//while parent have children
	while ((pid = wait(&child_status)) != -1) {

		//get polygon type by process id
		type = (pid == child_pids[0]) ? "hexagons" : (pid == child_pids[1]) ? "heptagons" : "octagons";
		
		if (WIFEXITED(child_status)) {
			printf("child terminated – created %d %s \n", WEXITSTATUS(child_status), type);
		}

		else {
			perror("Error in child: ");
			exit(1);
		}
	}
}

int main() {

	int input_fds[3];  //input file descriptors
	int output_fds[3]; //output file descriptors
	
	create_files(input_fds, output_fds); //input and output files
	get_input(input_fds); //get input and redirect it to files

	set_cursors_to_start(input_fds);	//set cursors back to start of the files
	run_childs(input_fds, output_fds);	//make childs and exec them

	return 0;
}