#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include "cachelab.h"
// isn't this just making a 2D array with structs that represent the valid bit - tag - bit offset and such..?

struct block
{
    int valid;// valid
    int tag;// tag 
    // no offset needed
    int access_counter;// LRU counter --> no matter you read or write, count is equal; counted together 
};

// allocate on write
// if its present in the cache, its a cache hit --> modify the cache
// if it is not present in the cache, its a cache miss --> add it to the cache
// if the cache is fucking full --> eviction is takes place according to LRU 

// Read and write can be implemented using the same funciton 


struct block** init_cache(int s_num, int e_num){
    int row_num = 2 << s_num;
    int col_num = e_num;
    struct block** res;
    int i, j; // loop counter
    //make space in memory
    res = (struct block**)malloc(row_num*sizeof(struct block*));
    for(i = 0; i < row_num; i++){
        res[i] = (struct block*)malloc(col_num*sizeof(struct block));
    }

    //fill array with zero value structs

    struct block zero;
    zero.valid=0;
    zero.tag= 0;
    zero.access_counter = 0;

    for(i = 0; i < row_num; i++){
        for(j = 0; j < col_num; j++){
            res[i][j] = zero;
        }
    }
    return res;
}
// use long long for 64bit address

void free_array(int s_num, int e_num, struct block** cache){
    int row_num = 2 << s_num;
    int i;
    for(i = 0; i < row_num; i++){
        free(cache[i]);
    }    
    free(cache);
}


char cache_access(long long address_raw, int set_bit_num, int block_bit_num, int row_length, struct block** cache, int counter){
   long long sb_offset = set_bit_num+block_bit_num;
   long long tag_mask = ((-1) << sb_offset);
   long long tag_comp = (address_raw & tag_mask) >> sb_offset;

   long long set_index_mask = ~((-1) << sb_offset);
   long long set_index = (address_raw & set_index_mask) >> block_bit_num;

   struct block* row = cache[set_index];
   struct block* target;
   int i;
   unsigned int LRU = ~0;
   int LRU_index;
   for(i=0; i < row_length; i++){
       target = &row[i];
       if(target->valid == 0){
           target->valid = 1;
           target->tag = tag_comp;
           target->access_counter=counter;
           return 'm';
       }else if(target->tag == tag_comp){
           //target->access_counter++;
           target->access_counter = counter;
           return 'h';
       }else{
           if(target->access_counter < LRU){
               LRU = target->access_counter;
               LRU_index = i;
           }
       }
   }
   (&row[LRU_index])->tag = tag_comp;
   (&row[LRU_index])->access_counter = counter;
   return 'e';
}


int main(int argc, char *argv[])
{
    int hit = 0;
    int miss = 0;
    int evic = 0;

    //////////
    char flags;
    int set_index_bit_num;
    int lines_per_set;
    int block_bit_num;
    char* trace_file_name;

    while ((flags=getopt(argc,argv,"s:E:b:t:")) != -1 )
    {
        switch (flags)
        {
        case 's':
            set_index_bit_num = atoi(optarg);
            break;
        case 'E':
            lines_per_set = atoi(optarg);
            break;
        case 'b':
            block_bit_num = atoi(optarg);
            break;
        case 't':
            trace_file_name = optarg;
            break;

        default:
            break;
        }
    }
    //initialize cache

    struct block** cache = init_cache(set_index_bit_num,lines_per_set);

    /////////////////////////// TEST CODE START ///////////////////////////////
    //printf("s: %d\n",set_index_bit_num);
    //printf("E: %d\n",lines_per_set);
    //printf("b: %d\n",block_bit_num);
    //printf("cache init zero: %d\n",cache[0][0].tag);
    /////////////////////////// TEST CODE END //////////////////////////////////


    char *line = NULL;
    size_t length;
    ssize_t read;
    FILE *trace_file;


    // open the trace file here
    trace_file = fopen(trace_file_name,"r"); 
    // ...
    int counter = 0;
    while((read = getline(&line, &length, trace_file)) != -1)
    {
        // each iteration, one line of the input file will be stored
        // in the variable "line"

        if(line[0] == ' ')
        {
            //printf("%s\n", line);
            // only 'I', the instruction load, doesn't have a space before it
            // do stuff to parse the line and call the functions
            
            //Parsing Line
            char* access_type = strtok(line, " ");
            long long address_raw = strtoull(strtok(strtok(NULL, line), ","), NULL,16);
            
            /////////////////////////// TEST CODE START ///////////////////////////////
            //printf("access type: %s\n", access_type);
            //printf("address raw: %d\n", address_raw);
            /////////////////////////// TEST CODE END //////////////////////////////////
            char res;
            //function read write 
            if(!(strcmp(access_type, "M"))){
                //call twice
                counter++;
                res = cache_access(address_raw, set_index_bit_num, block_bit_num, lines_per_set, cache, counter);
                switch (res)
                {
                case 'h':
                    hit++;
                    break;
                case 'm':
                    miss++;
                    break;
                case 'e':
                    evic++;
                    miss++;
                    break;                
                default:
                    break;
                }
                printf("%c ",res);
                counter++;
                res = cache_access(address_raw, set_index_bit_num, block_bit_num, lines_per_set, cache, counter);
                switch (res)
                {
                case 'h':
                    hit++;
                    break;
                case 'm':
                    miss++;
                    break;
                case 'e':
                    evic++;
                    miss++;
                    break;                
                default:
                    break;
                }
                printf("%c\n",res);
            }else{
                //call once
                counter++;
                res = cache_access(address_raw, set_index_bit_num, block_bit_num, lines_per_set, cache, counter);
                switch (res)
                {
                case 'h':
                    hit++;
                    break;
                case 'm':
                    miss++;
                    break;
                case 'e':
                    evic++;
                    miss++;
                    break;                
                default:
                    break;
                }
                printf("%c\n",res);
            }

        } 
    }     
    printSummary(hit, miss, evic);

    //free
    free_array(set_index_bit_num,lines_per_set,cache);
    return 0;
}
