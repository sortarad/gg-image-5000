#!/bin/sh

sdasz80 -plogff ./crt0.s 
mv ./crt0.rel ./obj/Release/
mv ./crt0.lst ./obj/Release/
