#include "crappergraphy.h"

void permute(const int size, unsigned int arr[size], int n, const int target, int current) {
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

unsigned int temp[] = 
{
    16132817U,12231425U,11314874U,410191439U,
    410191139U
};

int main( int argc, char **argv ) {
    
    const char *input_filename = argv[1];
    const char *output_filename = argv[2];
    
    long fsize;
    size_t target_bytes;

    FILE *fp = fopen(input_filename, "rb");

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);

    target_bytes = fsize / sizeof(unsigned int);
    unsigned int buffer[target_bytes];

    fseek(fp, 0, SEEK_SET);
    fread(buffer, sizeof(unsigned int), target_bytes, fp);
    fclose(fp);

    unsigned int result[target_bytes];

    char temp_iter = 0;

    for (char current_iter = 0; current_iter < 50; ++current_iter) {

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

    FILE * out_ptr = fopen(output_filename,"wb");
    fwrite(result,sizeof(unsigned int),target_bytes,out_ptr);
    fclose(out_ptr);

    return 0;
}