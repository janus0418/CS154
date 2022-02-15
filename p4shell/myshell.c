#define _GNU_SOURCE
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#define input_length 512
void myPrint(char *msg)
{
    write(STDOUT_FILENO, msg, strlen(msg));
}
void myError(int state){
    if (state == -1){
        char error_message[30] = "An error has occurred\n";
        write(STDOUT_FILENO, error_message, strlen(error_message));
    }
}
int parse_cmd_segment(char* cmd_segment, char** res){
    char* next;
    int i = 0;
    res[i] = strtok_r(cmd_segment, " \t", &next);
    while(res[i]!=NULL){
        i++;
        res[i] = strtok_r(NULL, " \t", &next);
    }
    return i;
}
int execute(char** indv_parsed_segment){
    if(!strcmp("exit",indv_parsed_segment[0])){
        if(indv_parsed_segment[1]!=NULL){
            myError(-1);
        }else{
            exit(0);
        }
    }else if(!strcmp("cd",indv_parsed_segment[0])){
        if(indv_parsed_segment[1]==NULL){
            chdir(getenv("HOME"));
        }else if(indv_parsed_segment[2]!=NULL){
            myError(-1);
        }else{
            int status = 0;
            status = chdir(indv_parsed_segment[1]);
            if(status < 0){
                myError(-1);
            }
        }
    }else if(!strcmp("pwd",indv_parsed_segment[0])){
        if(indv_parsed_segment[1]!=NULL){
            myError(-1);
        }else{
            char buffer[input_length];
            char* status = getcwd(buffer, input_length);
            if(status == NULL){
                myError(-1);
            }else{
                myPrint(status);
                myPrint("\n");            
            }
        }
    }else{
        if(fork()==0){
            int execute_status = 0;
            execute_status = execvp(indv_parsed_segment[0],indv_parsed_segment); 
            if(execute_status == -1){
                myError(-1);
                exit(0);
            }
        }else{
            wait(NULL);
        }
    }
    return 1;
}
// return -1 if error occurs, return 0 if there is no redireciton, return 1 if redirection happened
//If the command before redirection is a built-in command (for example, "cd > output"), you should also throw the error message.
// Built-in commands should be called without any redirection.
int check_redirection_and_if_exist_execute_redirection(char* cmd_segment){
    //check for redireciton
    int i = 0;
    int redirection_index = 0;
    int number_of_redirection = 0;
    int advanced_redirection_index = 0;
    int number_of_advanced_redireciton = 0;
    while(cmd_segment[i]!='\0'){
        if(cmd_segment[i]=='>'){ //must implement something to take care of advanced redirection case; need to count case of i== > && i+1 == +case 
            if(cmd_segment[i+1]=='+'){
                number_of_advanced_redireciton++;
                advanced_redirection_index = i;
                i++;
            }else{
                number_of_redirection++;
                redirection_index = i;
                i++;
            }
        }else{
            i++;
        }
    }
    if((number_of_redirection+number_of_advanced_redireciton) > 1){
        //printf("line 86");
        myError(-1);
        return -1;
    }else if(number_of_redirection==1){
        cmd_segment[redirection_index] = '\0';
        char* single_cmd_left_parsed[input_length];
        char* destination_right_parsed[input_length];
        char* single_cmd_left = cmd_segment;
        char* destination_right = (cmd_segment+redirection_index+1);
        parse_cmd_segment(single_cmd_left, single_cmd_left_parsed);
        parse_cmd_segment(destination_right, destination_right_parsed);
        if(single_cmd_left_parsed[0]==NULL||destination_right_parsed[0]==NULL){ //output file desitnation or cmd not given
            myError(-1);
            return -1;                
        }else if(destination_right_parsed[1]!=NULL){ //single_cmd_left_parsed[1]!=NULL||
            //printf("line 104");
            myError(-1);
            return -1;
        }else{
            char* single_cmd = single_cmd_left_parsed[0];
            int cd_exist = !strcmp(single_cmd, "cd");
            int pwd_exist = !strcmp(single_cmd, "pwd");
            int exit_exist = !strcmp(single_cmd, "exit");
            if(cd_exist||pwd_exist||exit_exist){ //making sure it throws error when a built in command is given 
                //printf("line 112");
                myError(-1);
                return -1;
            }
            int fd = open(destination_right_parsed[0], O_WRONLY|O_CREAT|O_EXCL|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP); 
            //sleep(1);
            if(fd < 0){ // if output file already exists before program run
                myError(-1);
                close(fd);
                return -1;
            }
            int stdout_copy = dup(STDOUT_FILENO);
            dup2(fd,STDOUT_FILENO);
            execute(single_cmd_left_parsed); 
            close(fd);
            dup2(stdout_copy,STDOUT_FILENO);
            return 1;
        }
    }else if(number_of_advanced_redireciton == 1){
      
        cmd_segment[advanced_redirection_index] = '\0'; // ls >+ temp ----> ls \0+ temp
        char* single_cmd_left_parsed[input_length];
        char* destination_right_parsed[input_length];
        char* single_cmd_left = cmd_segment;
        char* destination_right = (cmd_segment+advanced_redirection_index+2);
        parse_cmd_segment(single_cmd_left, single_cmd_left_parsed);
        parse_cmd_segment(destination_right, destination_right_parsed);
        if(single_cmd_left_parsed[0]==NULL||destination_right_parsed[0]==NULL){ // when there is no command 
            myError(-1);
            return -1;
        }else if(destination_right_parsed[1]!=NULL){ //when there is more than one command //single_cmd_left_parsed[1]!=NULL||
            myError(-1);
            return -1;
        }else{  // implementation --> work from here
            char* single_cmd = single_cmd_left_parsed[0];
            int cd_exist = !strcmp(single_cmd, "cd");
            int pwd_exist = !strcmp(single_cmd, "pwd");
            int exit_exist = !strcmp(single_cmd, "exit");
            if(cd_exist||pwd_exist||exit_exist){ //making sure it throws error when a built in command is given 
                myError(-1);
                return -1;
            }          
            int fd = open(destination_right_parsed[0], O_RDONLY);
            if(fd >= 0){
                // read loop
                int temp_fd = open(".temp", O_CREAT|O_RDWR|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP); 
                if(temp_fd < 0){
                    myError(-1);
                    close(fd);
                    return -1;
                }
                int stdout_copy = dup(STDOUT_FILENO);
                dup2(temp_fd,STDOUT_FILENO);
                execute(single_cmd_left_parsed); 
                dup2(stdout_copy,STDOUT_FILENO);
                //read from existing file and write it to temp file one character at a time
                int byte_read_status;
                /*
                do{
                    char t = 0;
                    byte_read_status = read(fd,&t,1);
                    write(temp_fd,&t,1);

                }while (byte_read_status != 0);
                */
                char t = 0;
                byte_read_status = read(fd,&t,1);
                while(byte_read_status != 0){
                    write(temp_fd,&t,1);
                    byte_read_status = read(fd,&t,1);
                }
                // rename temp file to existing file 
                rename(".temp",destination_right_parsed[0]);
                close(fd);
                close(temp_fd);
            }else{ //case when file isn't there
                //myPrint("file doesn't exist");
                close(fd);
                fd = open(destination_right_parsed[0], O_WRONLY|O_CREAT|O_EXCL|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP); 
                sleep(1);
                if(fd < 0){
                    //printf("line 207");
                    myError(-1);
                    close(fd);
                    return -1;
                }
                int stdout_copy = dup(STDOUT_FILENO);                
                dup2(fd,STDOUT_FILENO);
                execute(single_cmd_left_parsed); 
                close(fd);
                dup2(stdout_copy,STDOUT_FILENO);                
                return 1;                    
            }
        }
    }else{
        return 0;
    }
    return -1;
}
// array of array of strings -> pass each array of string to execute late
int parse_execute_wrap(char* cmd_buff){
    int i = 0;
    char* cmd_segment; 
    char* next_segments;
    char* indv_parsed_segment[input_length];
    cmd_segment = strtok_r(cmd_buff,";\n",&next_segments);
    /*
    if(cmd_segment == NULL){ // ; -> just this inputted case
        myError(-1);
    }
    */
    while(cmd_segment!=NULL){
        i++;
        // this should be where redireciton must fit in
        int redirection_process_status = check_redirection_and_if_exist_execute_redirection(cmd_segment);
        if(!redirection_process_status){ //if redirection returns 0, that is if no redireciton exists and it is error free, normal execute is done
            parse_cmd_segment(cmd_segment, indv_parsed_segment);
            if(indv_parsed_segment[0]==NULL){ // single sapce
                cmd_segment = strtok_r(NULL,";\n",&next_segments);
            }else{
                execute(indv_parsed_segment);
                cmd_segment = strtok_r(NULL,";\n",&next_segments);
            }
        }else{
            cmd_segment = strtok_r(NULL,";\n",&next_segments);
        }
    }
    return i;
}
int main(int argc, char *argv[]) 
{
    char cmd_buff[input_length+2]; // to check for illegal input do i make it input lenght + 1 and see if this is not null? 
    if(argc == 1){
        char* pinput;
        while (1) {
            myPrint("myshell> ");
            pinput = fgets(cmd_buff, input_length+2, stdin);
            if (!pinput) {
                exit(0); // shoudl there be an error here?
            }else{
                if(strlen(pinput)>=513 && cmd_buff[512]!='\n'){ // more than 512 inputs
                    while(fgets(cmd_buff,input_length+2,stdin)!=NULL){
                        if(cmd_buff[strlen(cmd_buff)-1]=='\n'){
                            break;
                        }
                    }
                    myError(-1);
                    continue;
                }
                parse_execute_wrap(cmd_buff);
            }
        }
    }else if(argc == 2){
        FILE* batch = fopen(argv[1],"r");
        char cmd_buff[input_length+2];
        if(batch == NULL){
            myError(-1);
            fclose(batch);
            exit(0);
        }
        char* pinput;
        //fgets(cmd_buff,input_length,batch)!=NULL
        while((pinput=fgets(cmd_buff,input_length+2,batch))!=NULL){
            if(!pinput){
                exit(0); // should there be an error here?
            }else{
                char* white_line_check[input_length];
                char* next;
                char cmd_buff_cpy[input_length+2];
                strcpy(cmd_buff_cpy,cmd_buff);
                white_line_check[0] = strtok_r(cmd_buff_cpy, " \t\n", &next);
                if(white_line_check[0]==NULL){
                    continue;
                }else{
                    myPrint(cmd_buff);
                    if(strlen(pinput)>=513 && cmd_buff[512]!='\n'){ // more than 512 inputs
                        while(fgets(cmd_buff,input_length+2,batch)!=NULL){
                            myPrint(cmd_buff);
                            if(cmd_buff[strlen(cmd_buff)-1]=='\n'){
                                break;
                            }
                        }
                        myError(-1);
                        continue;
                    }
                    parse_execute_wrap(cmd_buff);
                }
            }
        }
        fclose(batch);
        exit(0);
    }else{
        myError(-1);
        exit(0);
    }
}