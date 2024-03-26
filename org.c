#include "help.c"


void cipher(uint8_t** state, int Nr, uint8_t** w);

void invCipher(uint8_t** state, int Nr, uint8_t** w);

void mixColumns(uint8_t** state);

void invMixColumns(uint8_t** state);

void shiftRows(uint8_t** state);

void invShiftRows(uint8_t** state);

void subBytes(uint8_t* byte);

void subWord(uint8_t* bytes);

void subState(uint8_t** state);

void invSubState(uint8_t** state);

void addRoundKey(uint8_t** state, uint8_t** round_key);

void generateRoundConstants(uint8_t* Rcon, int Nr);

void generateInverseSBox();

void generateKey(uint8_t* key, int Nc);

void keyExpansion(uint8_t* key, uint8_t** round_keys, int Nk, int Nr);

void populateState(uint8_t** state);

void printState(uint8_t** state);

void rotWord(uint8_t* A);

uint8_t** initializeState(int n, int m);

uint8_t* allocateList(int size);

uint8_t xTimes(uint8_t a, uint8_t b);

void convertInput(char* input, uint8_t** state);

void convertOutput(uint8_t** state);

void stateToChunk(uint8_t** state, uint8_t* chunk);

void writeBytesToFile(uint8_t* bytes, size_t size, char* filename);

void readFile(char* filename);

void convertMessage();

void chunkToState(uint8_t* chunk, uint8_t** state);

void bytesToChunks(uint8_t* bytes, uint8_t** chunks, long num_chunks);

uint8_t* chunksToBytes(uint8_t** chunks, long num_chunks);

void initializeVariables(int key_size, int* Nk, int* Nr, int* Nc);

void writeUnencryptedBytesToFile(uint8_t* bytes, size_t size, char* filename);

void encryptFile(char* filepath, char* keypath, char* exportpath);

long getBytesInFile(char* filepath);

void decryptFile(char* filepath, char* keypath, char* exportpath);

void fileToBytes(char* filepath, uint8_t* bytes);

void temp();

void clear(uint8_t** A, int width);

void decryptFile(char* filepath, char* keypath, char* exportpath);

uint8_t* getNewKey(int key_length);

void exportKey(uint8_t* key, int key_length, char* filepath);

void exportNewKey(int key_length, char* filepath);

uint8_t* readKeyFromFile(char* keypath, int bytes_in_key);

void checkFiles(char* file1, char* file2);

long removePadding(uint8_t* bytes, int bytes_in_file);

long bytesToTotalChunks(long bytes);

void createFile(char* filepath, uint64_t size);

void printBytes(uint8_t* bytes, int size){
    printf("\n");
    for(int i = 0; i < size; i++){
        if(i % 16 == 0){printf("\n"); }
        printf("%02x", bytes[i]);
    }
    printf("\n");
}

void writeTextToFile(uint8_t* text, int size, char* filepath);

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
            case 5:
                createFile("demo.txt", 1024*1024);
                break;
            default:
                printf("\nInvalid Choice.\n");
        }
    }

    return 0;
}

void createFile(char* filepath, uint64_t size){
    FILE *file;

    file = fopen(filepath, "wb");
    if (file == NULL) {
        perror("error opening file");
        return;
    }

    uint8_t* bytes = allocateList(size);
    for(int i = 0; i < size; i++){
        bytes[i] = rand() % 256;
    }

    size_t bytes_written = fwrite(bytes, sizeof(uint8_t), size, file);
    if (bytes_written != size) {
        perror("error writing to file");
        fclose(file);
        return;
    }

    fclose(file);
}

uint8_t* getNewKey(int key_length){
    int Nc = key_length / 8;
    uint8_t* key = allocateList(Nc);
    generateKey(key, Nc);
    return key;
}

void exportNewKey(int key_length, char* filepath){
    uint8_t* key = getNewKey(key_length);
    exportKey(key, key_length, filepath);
    free(key);
}

void exportKey(uint8_t* key, int key_length, char* filepath){
    writeTextToFile(key, key_length / 8, filepath);
}

void writeTextToFile(uint8_t* text, int size, char* filepath){
    FILE *file = fopen(filepath, "w");
    if(file == NULL){
        perror("Error opening file");
        return;
    }

    for(int i = 0; i < size; i++){
        fprintf(file, "%02x", text[i]);
    }

    fclose(file);
}

void clear(uint8_t** A, int width){
    for(int i = 0; i < width; i++){
        free(A[i]);
    }
    free(A);
}

long getBytesInFile(char* filepath){
    FILE *file;

    long bytes_in_file;

    file = fopen(filepath, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    fseek(file, 0, SEEK_END);

    bytes_in_file = ftell(file);

    fclose(file);

    return bytes_in_file;
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

    uint8_t** round_keys = initializeState(4 * (Nr + 1), 4);
    keyExpansion(key, round_keys, Nk, Nr);

    uint8_t** state = initializeState(4, 4);
    uint8_t** encrypted_chunks = initializeState(num_chunks, 16);
    uint8_t** unencrypted_chunks = initializeState(num_chunks, 16);

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

void encryptFile(char* filepath, char* keypath, char* exportpath){

    checkFiles(filepath, keypath);

    long bytes_in_key = getBytesInFile(keypath) / 2;
    uint8_t* key = readKeyFromFile(keypath, bytes_in_key);

    int key_size = bytes_in_key * 8; // number of bits that make up the key
    int Nk = key_size / 32; // number of 32 bit chunks in key
    int Nr = Nk + 6; // number of rounds respective to the key_size
    int Nc = key_size / 8; // quantity of 8 bit numbers in key

    generateInverseSBox();

    uint8_t** round_keys = initializeState(4 * (Nr + 1), 4);
    keyExpansion(key, round_keys, Nk, Nr);

    long bytes_in_file = getBytesInFile(filepath);
    long num_chunks = bytesToTotalChunks(bytes_in_file);
    long total_bytes = num_chunks * 16;

    printf("\nNum chunks: %d\n", num_chunks);

    uint8_t padding = 16 - (bytes_in_file % 16);
    if(padding == 16){ padding = 0; }

    uint8_t* bytes = allocateList(num_chunks * 16);
    fileToBytes(filepath, bytes);

    for(int i = 0; i < padding; i++){
        bytes[total_bytes - (1 + i)] = padding;
    }

    uint8_t** state = initializeState(4, 4);
    uint8_t** chunks = initializeState(num_chunks, 16);
    uint8_t** encrypted_chunks = initializeState(num_chunks, 16);

    bytesToChunks(bytes, chunks, num_chunks);

    for(int i = 0; i < num_chunks; i++){
        chunkToState(chunks[i], state);
        cipher(state, Nr, round_keys);
        stateToChunk(state, encrypted_chunks[i]);
    }

    uint8_t* enc_bytes = chunksToBytes(encrypted_chunks, num_chunks);

    writeBytesToFile(enc_bytes, (size_t) total_bytes, exportpath);

    //decryptFile(exportpath, keypath, "bruh.txt");


    uint8_t* un_bytes = allocateList(total_bytes);
    uint8_t** masked_chunks = initializeState(num_chunks, 16);
    
    fileToBytes(exportpath, un_bytes);

    uint8_t** unencrypted_chunks = initializeState(num_chunks, 16);
    bytesToChunks(un_bytes, masked_chunks, num_chunks);

    for(int i = 0; i < num_chunks; i++){
        chunkToState(encrypted_chunks[i], state);
        invCipher(state, Nr, round_keys);
        stateToChunk(state, unencrypted_chunks[i]);
    }

    uint8_t* unenc_bytes = chunksToBytes(unencrypted_chunks, num_chunks);

    printBytes(unenc_bytes, 48);
     

    clear(round_keys, 4 * (Nr + 1));
    clear(chunks, num_chunks);
    clear(encrypted_chunks, num_chunks);
    clear(state, 4);
    free(enc_bytes);
    free(bytes);
    free(key);

    printf("\nSuccessfully encrypted file.\n");
}

long removePadding(uint8_t* bytes, int bytes_in_file){
    long last = bytes_in_file - 1;
    uint8_t padding = bytes[last];

    int is_padded = 1;
    for(long i = last; i > last - padding; i--){
        if(bytes[i] != padding){
            is_padded = 0;
            break;
        }
    }
    padding *= is_padded;

    long real_size = bytes_in_file;

    if(is_padded){
        real_size -= padding;
    }

    return real_size;
}

long bytesToTotalChunks(long bytes){
    int num_chunks = bytes / 16;
    if(bytes % 16 != 0){ num_chunks++; }
    return num_chunks;
}

void checkFiles(char* file1, char* file2){
    FILE *input = fopen(file1, "rb");
    if(input == NULL){
        perror("Error opening file");
        return;
    }
    fclose(input);

    FILE *keyfile = fopen(file2, "r");
    if(keyfile == NULL){
        perror("Error opening file");
        return;
    }
    fclose(keyfile);
}

uint8_t* readKeyFromFile(char* keypath, int bytes_in_key){

    FILE *keyfile = fopen(keypath, "r");
    int* temp_key = (int*)malloc(bytes_in_key * sizeof(int));
    for(int i = 0; i < bytes_in_key; i++){
        fscanf(keyfile, "%02x", &temp_key[i]);
    }
    fclose(keyfile);

    uint8_t* key = allocateList(bytes_in_key);
    for(int i = 0; i < bytes_in_key; i++){
        key[i] = (uint8_t) temp_key[i];
    }
    free(temp_key);

    return key;
}

void fileToBytes(char* filepath, uint8_t* bytes){
    int index = 0;
    unsigned char buffer;
    FILE *file = fopen(filepath, "rb");
    while(fread(&buffer, sizeof(buffer), 1, file) == 1){
        bytes[index++] = (uint8_t) buffer;
    }
    fclose(file);
}

uint8_t* chunksToBytes(uint8_t** chunks, long num_chunks){
    uint8_t* bytes = allocateList(num_chunks * 16);
    for(int i = 0; i < num_chunks; i++){
        for(int j = 0; j < 16; j++){
            bytes[16 * i + j] = chunks[i][j];
        }
    }
    return bytes;
}

void stateToChunk(uint8_t** state, uint8_t* chunk){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            chunk[4 * i + j] = state[i][j];
        }
    }
}

void bytesToChunks(uint8_t* bytes, uint8_t** chunks, long num_chunks){
    for(int i = 0; i < num_chunks; i++){
        for(int j = 0; j < 16; j++){
            chunks[i][j] = bytes[16 * i + j];
        }
    }
}

void chunkToState(uint8_t* chunk, uint8_t** state){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] = chunk[4 * i + j];
        }
    }
}

void readFile(char* filename){
    FILE *file;
    unsigned char buffer; // Buffer to hold one byte (8 bits)
    int bit_position; // Current position within the byte (0 to 7)

    // Open the file in binary mode for reading
    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    uint8_t bytes[1024];
    int index = 0;

    // Read bytes from the file until the end is reached
    while (fread(&buffer, sizeof(buffer), 1, file) == 1) {
        // Process each bit in the byte
        printf("%02x ", buffer);
        bytes[index++] = (uint8_t) buffer;
        /*
        for (bit_position = 7; bit_position >= 0; bit_position--) {
            // Extract the current bit from the byte
            int bit = (buffer >> bit_position) & 1;

            // Print or process the bit as needed
            printf("%d", bit);
        }
        */
    }

    // Close the file
    fclose(file);

    printf("\n");
    for(int i = 0; i < 33; i++){
        printf("%02x ", bytes[i]);
    }
    printf("\n");

    writeBytesToFile(bytes, (size_t) 33, "output.txt");
}

void writeUnencryptedBytesToFile(uint8_t* bytes, size_t size, char* filename){

    FILE *file = fopen(filename, "w");
    if(file == NULL){
        perror("Error opening file");
        return;
    }

    for(int i = 0; i < size; i++){
        fprintf(file, "%02x", bytes[i]);
    }

    fclose(file);

}

void writeBytesToFile(uint8_t* bytes, size_t size, char* filename){

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("error opening file");
        return;
    }

    size_t bytes_written = fwrite(bytes, sizeof(uint8_t), size, file);
    if (bytes_written != size) {
        perror("error writing to file");
        fclose(file);
        return;
    }

    fclose(file);
}

void convertOutput(uint8_t** state){

    char chunk[16];

    int size = 16 - state[3][3];

    for(int i = 0; i < size; i++){
        chunk[i] = (char) state[i / 4][i % 4];
    }

    printf("\n%s\n", chunk);

}

void convertInput(char* input, uint8_t** state){
    int sizeOfInput = 0;
    while(input[sizeOfInput++] != '\0'){}

    if(sizeOfInput % 16 != 0){
        uint8_t pad = 16 - sizeOfInput;
        for(int i = 0; i < 16; i++){
            int x = (int)i / 4, y = i % 4;
            if(i < sizeOfInput){
                state[i / 4][i % 4] =  (uint8_t) input[i];
            }else{
                state[i / 4][i % 4] = pad;
            }
        }
    }
}

void cipher(uint8_t** state, int Nr, uint8_t** w){
    addRoundKey(state, &w[0]);
    for(int i = 1; i < Nr - 1; i++){
        subState(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, &w[4 * i]);
    }
    subState(state);
    shiftRows(state);
    addRoundKey(state, &w[4 * Nr]);
}

void invCipher(uint8_t** state, int Nr, uint8_t** w){
    addRoundKey(state, &w[4 * Nr]);
    invShiftRows(state);
    invSubState(state);
    for(int i = Nr - 2; i > 0; i--){
        addRoundKey(state, &w[4 * i]);
        invMixColumns(state);
        invShiftRows(state);
        invSubState(state);
    }
    addRoundKey(state, &w[0]);
}

void mixColumns(uint8_t** state){
    uint8_t temp[4];

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            temp[j] = state[j][i];
        }
        for(int j = 0; j < 4; j++){
            state[j][i] = xTimes(mixing_matrix[j][0], temp[0]) ^ xTimes(mixing_matrix[j][1], temp[1]) ^ xTimes(mixing_matrix[j][2], temp[2]) ^ xTimes(mixing_matrix[j][3], temp[3]);
        }
    }
}

void invMixColumns(uint8_t** state){
    uint8_t temp[4];

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            temp[j] = state[j][i];
        }
        for(int j = 0; j < 4; j++){
            state[j][i] = xTimes(reverse_mixing_matrix[j][0], temp[0]) ^ xTimes(reverse_mixing_matrix[j][1], temp[1]) ^ xTimes(reverse_mixing_matrix[j][2], temp[2]) ^ xTimes(reverse_mixing_matrix[j][3], temp[3]);
        }
    }
}

void shiftRows(uint8_t** state){
    for(int i = 1; i < 4; i++){
        for(int j = 0; j < i; j++){
            rotWord(state[i]);
        }
    }
}

void invShiftRows(uint8_t** state){
    for(int i = 1; i  < 4; i++){
        for(int j = 0; j < 4 - i; j++){
            rotWord(state[i]);
        }
    }
}

void subBytes(uint8_t* byte){
    *byte = aes_sbox[*byte];
}

void subWord(uint8_t* bytes){
    for(int i = 0; i < 4; i++){
        bytes[i] = aes_sbox[bytes[i]];
    }
}

void subState(uint8_t** state){
    for(int i = 0; i < 4; i++){
        subWord(state[i]);
    }
}

void invSubState(uint8_t** state){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] = inverse_aes_box[state[i][j]];
        }
    }
}

void addRoundKey(uint8_t** state, uint8_t** round_key){
    for(int i = 0; i < 4; i++){
        for(int j  = 0; j < 4; j++){
            state[i][j] ^= round_key[i][j];
        }
    }
}

void generateRoundConstants(uint8_t* Rcon, int Nr){
    uint8_t temp = 0x01;
    Rcon[0] = temp;
    for(int i = 0; i < Nr - 1; i++){
        Rcon[i + 1] = xTimes(temp, 2);
        temp = Rcon[i + 1];
    }
}

void generateInverseSBox(){
    inverse_aes_box = allocateList(SBOX_SIZE);
    int x, y, z;
    for(int i = 0; i < SBOX_SIZE; i++){
        z = aes_sbox[i];
        inverse_aes_box[z] = i;
    }
}

void generateKey(uint8_t* key, int Nc){
    time_t t;

    srand((unsigned) time(&t));

    for(int i = 0; i < Nc; i++){
        key[i] = rand() % 256;
    }
}

void keyExpansion(uint8_t* key, uint8_t** round_keys, int Nk, int Nr){
    uint8_t* Rcon = allocateList(Nr);

    generateRoundConstants(Rcon, Nr);

    int i;

    for(i = 0; i <= Nk - 1; i++){
        for(int j = 0; j < 4; j++){
            round_keys[i][j] = key[4 * i + j];
        }
    }

    uint8_t temp[4];

    for(i; i < 4 * (Nr + 1); i++){
        for(int j = 0; j < 4; j++){
            temp[j] = round_keys[i - 1][j];
        }

        if(i % Nk == 0){ // g transform function
            rotWord(temp);
            subWord(temp);
            for(int j = 0; j < 4; j++){
                temp[j] ^= Rcon[(i / Nk) - 1];
            }
        }else if(Nk > 6 && i % Nk == 4){
            subWord(temp);
        }
        for(int j = 0; j < 4; j++){
            round_keys[i][j] = round_keys[i - Nk][j] ^ temp[j];
        }
    }

    free(Rcon);
}

void populateState(uint8_t** state){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] = rand() % 256;
        }
    }
}

void printState(uint8_t** state){
    printf("\n-----------");
    for(int i = 0; i < 4; i++){
        printf("\n");
        for(int j = 0; j < 4; j++){
            printf("%02x ", state[i][j]);
        }
    }
    printf("\n");
}

void rotWord(uint8_t* A){
    uint8_t first = A[0];

    for(int i = 0; i < 4 - 1; i++){
        A[i] = A[i + 1];
    }

    A[3] = first;
}

uint8_t** initializeState(int n, int m){
    uint8_t** state = (uint8_t**)malloc(n * sizeof(uint8_t*));
    
    for(int i = 0; i < n; i++){
        state[i] = (uint8_t*)malloc(m * sizeof(uint8_t));
    }

    return state;
}

uint8_t* allocateList(int size){
    uint8_t* A = (uint8_t*) malloc(size * sizeof(uint8_t));
    return A;
}

uint8_t xTimes(uint8_t a, uint8_t b){
    uint8_t result = 0;
    uint8_t carry;

    for(int i = 0; i < 8; i++){
        if(b & 1){
            result ^= a;
        }
        carry = a & 0x80;
        a<<=1;
        if(carry){
            a^=0x1b;
        }
        b>>=1;
    }
    return result;
}
