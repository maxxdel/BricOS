void kernel_main() {
    char* video_memory = (char*) 0xB8000;

    char* sentence = "Hello World";
    int i = 0;

    while (sentence[i] != '\0') {
        video_memory[i * 2] = sentence[i];     // Character
        video_memory[i * 2 + 1] = 0x07;        // Color byte
        i++;
    }

    while (1) {}
}

