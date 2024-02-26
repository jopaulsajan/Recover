#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Unable to open file: %s\n", argv[1]);
        return 1;
    }

    uint8_t buffer[512];
    int jpeg_count = 0;

    char storefile[8];
    FILE *output_file = NULL;

    while (fread(buffer, 1, 512, file) == 512)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (output_file != NULL)
            {
                fclose(output_file);
            }

            sprintf(storefile, "%03i.jpg", jpeg_count);
            output_file = fopen(storefile, "w");

            if (output_file == NULL)
            {
                fclose(file);
                fprintf(stderr, "Could not create output JPEG file %s\n", storefile);
                return 1;
            }

            jpeg_count++;
        }

        if (output_file != NULL)
        {
            fwrite(buffer, 1, 512, output_file);
        }
    }

    if (output_file != NULL)
    {
        fclose(output_file);
    }

    fclose(file);
    return 0;
}
