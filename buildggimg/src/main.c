#include <stdio.h>
#include <stdlib.h>

#include "argparse.h"

// #define LOG_LEVEL   DEBUG
#include "seethe.h"

#include "gamegear_image_converter.h"

static const char *const usage[] = {
    "buildggimg [options] [[--] args]",
    "buildggimg [options]",
    NULL
};

int main(int argc, const char **argv)
{
    char *output = NULL;
    char *image = NULL;
    char *ggfile = NULL;
    int result = 0;
    int temp;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_STRING('o', "output", &output, "output filename"),
        OPT_STRING('i', "image", &image, "test only"),
        OPT_STRING('g', "ggfile", &ggfile, "path to read"),
        OPT_END()
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usage, 0);
    argparse_describe(
        &argparse,
        "\nProgram to create a gamegear gg file to display an image.",
        "\n(All file parameters are required).");
    argc = argparse_parse(&argparse, argc, argv);

    if ((output == NULL) || (image == NULL) || (ggfile == NULL))
    {
        argparse_usage(&argparse);
        result = -1;
        goto done;
    }

    info("Output filename: %s",output);
    info("Image filename: %s",image);
    info("Game gear filename: %s",ggfile);

    temp = gamegear_image_converter_from_file(image,ggfile,output);
    if (temp != 0)
    {
        result = temp;
        goto done;
    }

    info("Done!");

done:
    // free(output);
    // free(image);
    // free(ggfile);

    return result;
}

