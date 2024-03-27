#include "algoFunctions.c"


void generateKey(uint8_t* key, int Nc);

void encryptFile(char* filepath, char* keypath, char* exportpath);

void decryptFile(char* filepath, char* keypath, char* exportpath);

int main(){

    clock_t start, end;
    double cpu_time_used;

    int action;
    int key_option;

    char filepath[255], keypath[255], exportpath[255];

    int exit = 1;

    while(exit){

        printf("\n1) Generate Key\n\n2) Encrypt File\n\n3) Decrypt File\n\n4) Quit\n\n");
        printf("Enter choice: ");

        scanf("%d", &action);

        switch(action){
            case 1:
                printf("----------------\n\n");
                printf("1) 128\n\n2) 192\n\n3) 256\n\n");
                printf("Enter key size: ");
                scanf("%d", &key_option);
                printf("\nEnter file to export to: ");
                scanf("%s", &keypath);
                if(key_option > 0 && key_option < 4){
                    int key_lengths[] = {128, 192, 256};
                    exportNewKey(key_lengths[key_option - 1], keypath);
                    printf("\nKey successfully exported.\n");
                }else{
                    printf("\nInvalid choice.\n");
                }
                break;
            case 2:
                printf("\nEnter file path: ");
                scanf("%s", &filepath);
                printf("\nEnter key path: ");
                scanf("%s", &keypath);
                printf("\nEnter file to export to: ");
                scanf("%s", &exportpath);
                start = clock();
                encryptFile(filepath, keypath, exportpath);
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("\nFile encrypted in %f seconds.\n", cpu_time_used);
                break;
            case 3:
                printf("\nEnter file path: ");
                scanf("%s", &filepath);
                printf("\nEnter key path: ");
                scanf("%s", &keypath);
                printf("\nEnter file to export to: ");
                scanf("%s", &exportpath);
                start=clock();
                decryptFile(filepath, keypath, exportpath);
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("\nFile decrypted successfully in %f seconds.\n", cpu_time_used);
                break;
            case 4:
                exit = 0;
                break;
            default:
                printf("\nInvalid Choice.\n");
        }
    }

    return 0;
}

void encryptFile(char* filepath, char* keypath, char* exportpath){
    checkFiles(filepath, keypath);

    int bytes_in_key = getBytesInFile(keypath) / 2;
    uint8_t* key = readKeyFromFile(keypath, bytes_in_key);

    int key_size = bytes_in_key * 8; // number of bits that make up the key
    int Nk = key_size / 32; // number of 32 bit chunks in key
    int Nr = Nk + 6; // number of rounds respective to the key_size
    int Nc = key_size / 8; // quantity of 8 bit numbers in key

    generateInverseSBox();

    uint8_t** round_keys = initializeMatrix(4 * (Nr + 1), 4);
    keyExpansion(key, round_keys, Nk, Nr);
    free(key);

    long bytes_in_file = getBytesInFile(filepath);
    long num_chunks = bytesToTotalChunks(bytes_in_file);
    long total_bytes = num_chunks * 16;

    uint8_t padding = 16 - (bytes_in_file % 16);
    if(padding == 16){ padding = 0; }

    uint8_t* bytes = allocateList(num_chunks * 16);
    fileToBytes(filepath, bytes);

    for(int i = 0; i < padding; i++){
        bytes[total_bytes - (1 + i)] = padding;
    }

    uint8_t** state = initializeMatrix(4, 4);
    uint8_t** chunks = initializeMatrix(num_chunks, 16);
    uint8_t** encrypted_chunks = initializeMatrix(num_chunks, 16);
    bytesToChunks(bytes, chunks, num_chunks);
    free(bytes);

    for(int i = 0; i < num_chunks; i++){
        chunkToState(chunks[i], state);
        cipher(state, Nr, round_keys);
        stateToChunk(state, encrypted_chunks[i]);
    }
    clear(state, 4);
    clear(chunks, num_chunks);
    clear(round_keys, 4 * (Nr + 1));

    uint8_t* enc_bytes = chunksToBytes(encrypted_chunks, num_chunks);
    
    clear(encrypted_chunks, num_chunks);

    writeBytesToFile(enc_bytes, (size_t) total_bytes, exportpath);

    printf("\nSuccessfully encrypted file.\n");
}

void decryptFile(char* filepath, char* keypath, char* exportpath){

    checkFiles(filepath, keypath);

    long bytes_in_key = getBytesInFile(keypath) / 2;

    uint8_t* key = readKeyFromFile(keypath, bytes_in_key);

    long bytes_in_file = getBytesInFile(filepath);
    long num_chunks = bytesToTotalChunks(bytes_in_file);

    uint8_t* bytes = allocateList(bytes_in_file);
    fileToBytes(filepath, bytes);

    int key_size = bytes_in_key * 8; // number of bits that make up the key
    int Nk = key_size / 32; // number of 32 bit chunks in key
    int Nr = Nk + 6; // number of rounds respective to the key_size
    int Nc = key_size / 8; // quantity of 8 bit numbers in key

    generateInverseSBox();

    uint8_t** round_keys = initializeMatrix(4 * (Nr + 1), 4);
    keyExpansion(key, round_keys, Nk, Nr);

    uint8_t** state = initializeMatrix(4, 4);
    uint8_t** encrypted_chunks = initializeMatrix(num_chunks, 16);
    uint8_t** unencrypted_chunks = initializeMatrix(num_chunks, 16);

    uint8_t* encrypted_bytes = allocateList(num_chunks * 16);
    fileToBytes(filepath, encrypted_bytes);
    bytesToChunks(encrypted_bytes, encrypted_chunks, num_chunks);

    for(int i = 0; i < num_chunks; i++){
        chunkToState(encrypted_chunks[i], state);
        invCipher(state, Nr, round_keys);
        stateToChunk(state, unencrypted_chunks[i]);
    }

    uint8_t* unencrypted_bytes = chunksToBytes(unencrypted_chunks, num_chunks);

    long final_size = removePadding(unencrypted_bytes, num_chunks * 16);

    writeBytesToFile(unencrypted_bytes, final_size, exportpath);

    free(key);
    free(bytes);
    free(encrypted_bytes);
    clear(round_keys, 4 * (Nr + 1));
    clear(unencrypted_chunks, num_chunks);
    clear(encrypted_chunks, num_chunks);
    clear(state, 4);
}