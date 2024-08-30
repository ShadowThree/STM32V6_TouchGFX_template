@echo off
if not exist Image_Bin (
    md Image_Bin
)

if not exist %1 (
    echo ERR: the DIR[%1] is not exist!
    pause
) else (
    python .\hex2bin.py -r 70000000: .\%1\%1.hex > .\Image_Bin\images.bin
)