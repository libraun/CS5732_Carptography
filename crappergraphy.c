#include "crappergraphy.h"

/*  @ function "permute" 

    @ param "size" The size of the array to be permuted.
    @ param "arr" The array (of size 'size') to be permuted.
    @ param "n" The current index of the array. LEAVE AS 0
    @ param "target" The target permutation for this array (for rotation).
    @ param "current" The current permutation for this array. LEAVE AS 0
*/
void permute(const int size, 
             unsigned int arr[size], 
             int n, 
             const int target, 
             int current) {

    for (size_t i = n+1; i < size; ++i) {
        char c = arr[n];

        arr[n] = arr[i]; 
        arr[i] = c;

        if (current == target) {
            break;
        } else {
            permute(size,arr, n+1, target, ++current);
        }
    }
}

int main( int argc, char **argv ) {
    
    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    /* Initializing a temporary vector as internal state */
    unsigned int temp[] = 
    {
        16132817U,12231425U,11314874U,410191439U,
        410191139U
    };
    
    long fsize;
    size_t target_bytes; // The number of blocks to be allocated

    /* Opening the file and taking filesize,*/
    FILE *fp = fopen(input_filename, "rb");

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);

    target_bytes = fsize / sizeof(unsigned int);

    /* Declare buffer as the number of n-byte sized blocks 
       to be used as initial storage */
    unsigned int buffer[target_bytes];

    /* Return to beginning to read file contents into buffer, then close file */
    fseek(fp, 0, SEEK_SET);
    fread(buffer, sizeof(unsigned int), target_bytes, fp);
    fclose(fp);

    /* The product of buffer XO*/
    unsigned int result[target_bytes];

    /* We use 'temp_iter' to specify how many times we want
       to permute the array (0 being 1 time for some reason) a given round.
       It rotates between 0 and the size of temp. */
    char temp_iter = 0;
    for (char current_iter = 0; current_iter < 50; ++current_iter) {
        
        /* Permute temp "temp_iter" times */
        permute(sizeof(temp) / sizeof(unsigned int),temp,0,temp_iter,0);
        if (temp_iter % 4 == 0) {
            temp_iter = 0;
        }
        int input_idx = 0;
        int temp_idx = 0;
        while (input_idx < target_bytes) {
            
            if (temp_idx % 5 == 0) {
                temp_idx = 0;
            }
            result[input_idx] = (current_iter == 0) ? 
                buffer[input_idx] ^ temp[temp_idx] : result[temp_idx] ^ temp[temp_idx];
                
            ++temp_idx; ++input_idx;
        }
        ++temp_iter;
    }

    /* Write file to output_filename */
    FILE * out_ptr = fopen(output_filename,"wb");
    fwrite(result,sizeof(unsigned int),target_bytes,out_ptr);
    fclose(out_ptr);

    return 0;
}