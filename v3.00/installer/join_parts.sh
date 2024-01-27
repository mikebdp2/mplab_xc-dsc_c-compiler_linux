#!/bin/sh
printf "=== Removing an old archive if it exists\n"
                                                          rm -f "./xc-dsc-v3.00-full-install-linux64-installer.run"
printf "=== Making a new archive from its parts\n"
                                                          touch "./xc-dsc-v3.00-full-install-linux64-installer.run"
cat "./xc-dsc-v3.00-full-install-linux64-installer.run_part01" >> "./xc-dsc-v3.00-full-install-linux64-installer.run"
cat "./xc-dsc-v3.00-full-install-linux64-installer.run_part02" >> "./xc-dsc-v3.00-full-install-linux64-installer.run"
cat "./xc-dsc-v3.00-full-install-linux64-installer.run_part03" >> "./xc-dsc-v3.00-full-install-linux64-installer.run"
printf "=== Doing a sync command (just in case)\n"
sync
printf "=== Finding a sha256sum of this archive\n"
sha256sum_correct="d76f445177c540fda9ddb76844b225650aa0f83cb96492c18c770b6f1b30e1bc  ./xc-dsc-v3.00-full-install-linux64-installer.run"
sha256sum_my=$(sha256sum "./xc-dsc-v3.00-full-install-linux64-installer.run")
printf "=== sha256sum should be\n$sha256sum_correct\n"
if [ "$sha256sum_my" = "$sha256sum_correct" ] ; then
    printf "^^^ this is correct, you can use a ./xc-dsc-v3.00-full-install-linux64-installer.run now...\n"
                                     chmod +x "./xc-dsc-v3.00-full-install-linux64-installer.run"
    exit 0
else
    printf "^^^ ! MISMATCH ! Check sha256sum manually: sha256sum ./xc-dsc-v3.00-full-install-linux64-installer.run\n"
    exit 1
fi
###
