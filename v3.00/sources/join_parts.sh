#!/bin/sh
printf "=== Removing an old archive if it exists\n"
                          rm -f "./v3.00.src.zip"
printf "=== Making a new archive from its parts\n"
                          touch "./v3.00.src.zip"
cat "./v3.00.src.zip_part01" >> "./v3.00.src.zip"
cat "./v3.00.src.zip_part02" >> "./v3.00.src.zip"
cat "./v3.00.src.zip_part03" >> "./v3.00.src.zip"
cat "./v3.00.src.zip_part04" >> "./v3.00.src.zip"
cat "./v3.00.src.zip_part05" >> "./v3.00.src.zip"
cat "./v3.00.src.zip_part06" >> "./v3.00.src.zip"
cat "./v3.00.src.zip_part07" >> "./v3.00.src.zip"
cat "./v3.00.src.zip_part08" >> "./v3.00.src.zip"
cat "./v3.00.src.zip_part09" >> "./v3.00.src.zip"
printf "=== Doing a sync command (just in case)\n"
sync
printf "=== Finding a sha256sum of this archive\n"
sha256sum_correct="199ca2e8f8b47dda1b7e15c4ec07ee56430a5f7b95cf9dc5f536f64cf697b477  ./v3.00.src.zip"
sha256sum_my=$(sha256sum "./v3.00.src.zip")
printf "=== sha256sum should be\n$sha256sum_correct\n"
if [ "$sha256sum_my" = "$sha256sum_correct" ] ; then
    printf "^^^ this is correct, you can use a ./v3.00.src.zip now...\n"
    exit 0
else
    printf "^^^ ! MISMATCH ! Check sha256sum manually: sha256sum ./v3.00.src.zip\n"
    exit 1
fi
###
