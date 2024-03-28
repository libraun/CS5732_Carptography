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
        11028137U, 1223014125U, 111420874U, 11914309U,
        1100139U, 1327301425U, 1513281037U, 187040U
    };
    
    long fsize;
    size_t target_bytes; // The number of blocks to be allocated

    /* Opening the file and taking filesize */
    FILE *fp = fopen(input_filename, "rb");
    if (fp == NULL) {
        printf("ERROR: Problem opening file '%s'; quitting.\n",input_filename);
        return EXIT_FAILURE;
    }
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);

    if (fsize == 0) {
        printf("ERROR: file must be non-empty; quitting.\n");
        fclose(fp);
        return EXIT_FAILURE;
    }
    target_bytes = ceil(fsize / sizeof(unsigned int));
    /* Declare buffer as the number of n-byte sized blocks 
       to be used as initial storage */
    unsigned int buffer[target_bytes];

    /* Return to beginning to read file contents into buffer, then close file */
    fseek(fp, 0, SEEK_SET);
    fread(&buffer, 1, fsize, fp);
    fclose(fp);
    /* The product of buffer XO*/
    unsigned int result[target_bytes];
    /* We use 'temp_iter' to specify how many times we want
       to permute the array (0 being 1 time for some reason) a given round.
       It rotates between 0 and the size of temp. */
    int temp_iter = 0;
    for (int current_iter = 0; current_iter < 1000; ++current_iter) {
        if (temp_iter % 63 == 0) {
            temp_iter = 0;
        }
        int input_idx = 0;
        int temp_idx = 0;
        while (input_idx < target_bytes+1) {
            /* Permute temp "temp_iter" times */
            permute(4,&temp[temp_idx], 0,temp_iter, 0);
            if (temp_idx % 8 == 0) {
                temp_idx = 0;
            }
            result[input_idx] = (current_iter == 0) ? 
                buffer[input_idx] ^ temp[temp_idx] : result[input_idx] ^ temp[temp_idx];
            ++temp_idx; 
            ++input_idx;
        }
        ++temp_iter;
    }

    /* Write contents of 'result' to file specified by output_filename */
    FILE * out_ptr = fopen(output_filename,"wb");

    if (out_ptr == NULL) {
        printf("ERROR: Problem opening output file '%s'; quitting.\n",output_filename);
        return EXIT_FAILURE;
    }

    fwrite(result,1,fsize,out_ptr);
    fclose(out_ptr);

    return EXIT_SUCCESS;
}