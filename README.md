# Gamegear image converter and display tool

## Quick Introduction for converting images

### Installation

#### Pre-requisites

* Install Imagemagick from the website: https://imagemagick.org/script/download.php (eg download file **ImageMagick-7.1.0-22-Q16-HDRI-x64-dll.exe** and install)

### Converting an image

To convert an image, use the **convert.bat** script. 

For example:

```
convert.bat sunflower.jpg sunflower-160x144.gg
```

The batch file will call magick.exe with the appropriate parameters to convert the image to a smaller image that can be used to display on the Gamegear. The tool buildggimg.exe will patch the image into a base gg file called testpatterncb.gg and produce a resulting patched file called sunflower-160x144.gg that can be opened in the emulator to display.

## Source codes

### buildggimg

The file **buildggimg.zip** contains the Visual Studio project to build the **buildggimg.exe** file. All this file does is take an RGB image of the size 160x144 pixels and converts and patches it into a base gg file. The result is written to an output file.

### Examples for using buildgging.exe

#### Help

```
buildggimg.exe --help
```

produces:
```
Usage: buildggimg [options] [[--] args]
   or: buildggimg [options]

Program to create a gamegear gg file to display an image.

    -h, --help            show this help message and exit

Basic options
    -o, --output=<str>    output filename
    -i, --image=<str>     test only
    -g, --ggfile=<str>    path to read

(All file parameters are required).
```

#### Converting using buildggimg.exe

Example conversion:
```
buildggimg.exe -g testpatterncb.gg -o sunflower-160x144.gg -i temp.rgb
```

### testpatterncb

The testpatterncp.zip contains the source code for the testpatterncb.gg file. The testpatterncb code is built using the SDCC (small device C compiler) and the CodeBlocks IDE.

#### Pre-requisites

To build the testpatterncb.gg image, the following tools need to be installed:

* SDCC (Small device C Compiler), from http://sdcc.sourceforge.net
* CodeBlocks IDE, from https://www.codeblocks.org

