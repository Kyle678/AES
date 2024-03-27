#include "matrixHandling.c"

void writeTextToFile(uint8_t* text, int size, char* filepath);

void writeBytesToFile(uint8_t* bytes, size_t size, char* filename);

void fileToBytes(char* filepath, uint8_t* bytes);

void createFile(char* filepath, uint64_t size);

void checkFiles(char* file1, char* file2);

void exportKey(uint8_t* key, int key_length, char* filepath);

long getBytesInFile(char* filepath);

uint8_t* readKeyFromFile(char* keypath, int bytes_in_key);




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

void fileToBytes(char* filepath, uint8_t* bytes){
    int index = 0;
    unsigned char buffer;
    FILE *file = fopen(filepath, "rb");
    while(fread(&buffer, sizeof(buffer), 1, file) == 1){
        bytes[index++] = (uint8_t) buffer;
    }
    fclose(file);
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

uint8_t* readKeyFromFile(char* keypath, int bytes_in_key){

    unsigned int* temp_key = (unsigned int*)malloc(bytes_in_key * sizeof(unsigned int));
    uint8_t* key = allocateList(bytes_in_key);
    FILE *file;

    file = fopen(keypath, "r");
    if(file == NULL){
        perror("Error opening file");
        return key;
    }

    uint8_t index = 0;

    while(fscanf(file, "%02x", &temp_key[index++]) == 1) {}

    fclose(file);

    for(int i = 0; i < bytes_in_key; i++){
        key[i] = temp_key[i];
    }
    free(temp_key);

    return key;
}

void exportKey(uint8_t* key, int key_length, char* filepath){
    writeTextToFile(key, key_length / 8, filepath);
}