#!/bin/bash

convert -resize 160x144! -colors 15 ../images/lonely-japanese-cherry-400x300.png ../images/lonely-japanese-cherry-160x144.rgb
../build/buildggimg -g ../images/testpatterncb.gg -o ../images/testpatterncb_output.gg -i ../images/lonely-japanese-cherry-160x144.rgb


