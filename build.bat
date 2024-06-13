make
bannertool.exe makebanner -i banner.png -a audio.wav -o banner.bnr
bannertool.exe makesmdh -s "Pong Pokemon" -l "A Pong Pokemon game" -p "Sunrase" -i icon.png  -o icon.icn
makerom -f cia -o Pong_Pokemon.cia -DAPP_ENCRYPTED=false -rsf Pong_Pokemon.rsf -target t -exefslogo -elf Pong_Pokemon.elf -icon icon.icn -banner banner.bnr
makerom -f cci -o Pong_Pokemon.3ds -DAPP_ENCRYPTED=true -rsf Pong_Pokemon.rsf -target t -exefslogo -elf Pong_Pokemon.elf -icon icon.icn -banner banner.bnr
pause > nul