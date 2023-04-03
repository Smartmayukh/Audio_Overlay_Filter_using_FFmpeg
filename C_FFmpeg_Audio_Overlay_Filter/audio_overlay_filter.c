#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>


int main() {
    // Convert input files to raw format using FFmpeg
    int ret = system("ffmpeg -y -i base_audio.wav -f s16le -ac 1 input.raw");
    if (ret != 0) {
        printf("Error converting input file.\n");
        return 1;
    }
    ret = system("ffmpeg -y -i overlay_audio.wav -f s16le -ac 1 -af atrim=start=1 overlay.raw");
    if (ret != 0) {
        printf("Error converting overlay file.\n");
        return 1;
    }


    // Open input and output files
    FILE *input_file = fopen("input.raw", "rb");
    if (!input_file) {
        printf("Error opening input file.\n");
        return 1;
    }
    FILE *overlay_file = fopen("overlay.raw", "rb");
    if (!overlay_file) {
        printf("Error opening overlay file.\n");
        return 1;
    }
    FILE *output_file = fopen("output.raw", "wb");
    if (!output_file) {
        printf("Error opening output file.\n");
        return 1;
    }


    // Set position and loop time
    int position = 61700; // position in milliseconds
    int loop_time = 1; // number of times to loop the overlay


    // Set gain values
    double gain_of_base = 0.2; // gain of base audio
    double gain_of_overlay = 1; // gain of overlay audio


    // Set silent flag
    int silent = 1; // make base audio silent while overlaying


    // Calculate position in samples
    int sample_rate = 44100; // assume sample rate of 44100 Hz
    int position_samples = (int)(position / 1000.0 * sample_rate);


    // Create a buffer for the net overlay audio
    fseek(overlay_file, 0, SEEK_END);
    long overlay_size = ftell(overlay_file);
    fseek(overlay_file, 0, SEEK_SET);
    int16_t *overlay_buffer = malloc(overlay_size * loop_time);
   
    // Fill the buffer with concatenated overlay samples
    for (int i = 0; i < loop_time; i++) {
        fread(overlay_buffer + i * (overlay_size / sizeof(int16_t)), sizeof(int16_t), overlay_size / sizeof(int16_t), overlay_file);
        fseek(overlay_file, 0, SEEK_SET);
    }


    // Read, modify and write one sample at a time
    int16_t input_sample, output_sample;
    int count;
    int n = 0; // sample counter
    int overlay_index = 0; // index for overlay buffer


    while(1) {
        count = fread(&input_sample, 2, 1, input_file); // read one 2-byte sample from input file
        if (count != 1) break;


        if (n >= position_samples && overlay_index < overlay_size * loop_time / sizeof(int16_t)) { // if position is reached and overlay buffer is not exhausted
            if (silent) { // make base audio silent while overlaying
                output_sample = (int16_t)(overlay_buffer[overlay_index] * gain_of_overlay);
            } else { // mix the two samples together with gain applied
                output_sample = (int16_t)(input_sample * gain_of_base + overlay_buffer[overlay_index] * gain_of_overlay) / 2;
            }
            overlay_index++; // increment overlay buffer index
        } else { // use input sample only
            output_sample = (int16_t)(input_sample);
        }


        fwrite(&output_sample, 2, 1, output_file);
        n++; // increment sample counter
    }


    // Close input and output files
    fclose(input_file);
    fclose(overlay_file);
    fclose(output_file);


    // Free the overlay buffer
    free(overlay_buffer);


    // Convert output file to WAV format using FFmpeg
    ret = system("ffmpeg -y -f s16le -ar 44100 -ac 1 -i output.raw out.wav");
    if (ret != 0) {
        printf("Error converting output file.\n");
        return 1;
    }


    return 0;
}
