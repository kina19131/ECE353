#include "common.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h> 
#include <dirent.h>
#include <string.h> 

#include <errno.h>

/* make sure to use syserror() when a system call fails. see common.h */

void usage(){
	fprintf(stderr, "Usage: cpr srcdir dstdir\n");
	exit(1);
}


void cpyFile(char* src, char* dest, int src_mode){
	//OPEN
	int fd_read;
	fd_read = open(src, O_RDONLY);
	if ( fd_read < 0 ) {
		syserror(read, src);
	}	
	
	//CREAT to WRITE 
	int fd_write; 
	fd_write = open(dest, O_CREAT|O_WRONLY|O_TRUNC);
	if (fd_write < 0){
		syserror(open, dest);
	}
	int chgMode; 
	if ((chgMode = chmod(dest, src_mode)) <0){
		syserror(chmod, dest); 
	}

	//if the mode is -rw-r--r--
	if (chgMode == 644){
		if ((chgMode = chmod(src, 755)) < 0){
			syserror(chmod, src); 
		}

		cpyFile(src, dest, 644);

	}	

	//READING & WRITING
	char buf[4096]; //read var
	int ret; //read var 
	ssize_t writ; //write var
	
	while ((ret = read(fd_read, buf, 4096)) != 0){ // return 0 when has ran all the bytes in the file	
		if( (ret <0) ){
			syserror(read, src); 
		}
	       	if ((writ = write(fd_write, buf, ret))<0){
			syserror(write, dest); 
		}	
	}
	 
	//CLOSE
	if(close(fd_read) < 0){
		syserror(close, src);
	}	
	if(close(fd_write) <0){
		syserror(close, dest); 
	}
}

void cpyDir(char* src, char* dest){
	DIR *dir; 
	struct dirent *dp; 
	struct stat statbuf; 

	if ((dir = opendir(src)) < 0){
		syserror(opendir, src); 
	}

	while((dp = readdir(dir)) != NULL){

		// if name of directory is "." or "..", continue
		if(strcmp(dp->d_name,".") == 0 || strcmp(dp->d_name, "..") == 0){
			continue; 
		}
		
		//path to the file or dir currently observing
		char new_src_path[100]; 
		strcpy(new_src_path, src); 
		strcat(new_src_path, "/");
		strcat(new_src_path, dp->d_name); 

		char new_dest_path[100]; 
		strcpy(new_dest_path, dest); 
		strcat(new_dest_path, "/");
		strcat(new_dest_path, dp->d_name); 

		//Get entry's information
		if(stat(new_src_path, &statbuf) < 0){
			syserror(stat, new_src_path); 
		}		

		//Copying functions 
		if (S_ISREG(statbuf.st_mode) > 0){ //if is file 
			cpyFile(new_src_path, new_dest_path, statbuf.st_mode); //calling cpyFile
		}

		else if (S_ISDIR(statbuf.st_mode) > 0){ //if is dir 
			if ((mkdir(new_dest_path, S_IRWXU | S_IRWXG | S_IROTH |S_IXOTH)) <0){
				syserror(mkdir, new_dest_path); 
			}
			cpyDir(new_src_path, new_dest_path); //calling cpyDir


			if ((chmod(new_dest_path, statbuf.st_mode)) < 0){ //handling the access mode
				syserror(chmod, new_dest_path); 
			}
		}

	}
	closedir(dir);
}

int main(int argc, char *argv[]){
	struct stat status; 

	if (argc != 3) {
		usage();
	}
	
	
	//get mode of src
	if ((stat(argv[1], &status)) < 0){
		syserror(stat, argv[1]);
	}
	
	if ((mkdir(argv[2], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) < 0){
		syserror(mkdir, argv[2]); //creating dest directory
	}

	if ((chmod(argv[2], status.st_mode)) < 0){
		syserror(chmod, argv[2]); 
	}

	cpyDir(argv[1], argv[2]);
	return 0;
}
