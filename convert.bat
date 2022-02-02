magick.exe %1 -resize 160x144! -colors 16 temp.rgb
buildggimg.exe -g testpatterncb.gg -o %2 -i temp.rgb


