# Setup #
    %PathToExe% [number of threads >0] [mode 1 or 0] [key number(virtual key code)] [filter (windivert filter syntax)]
# Useful examples #
Mobs will not see you and mechanics won't work on ya (3 minute limit, kick after)

    %PathToExe% 12 1 0 192 "udp.SrcPort == 3074"
Avtivate right before Orbs spawn and they will be duplicated, activate in time with shot in pvp and you will fire multiple shots

    %PathToExe% 12 1 0 192 "udp.SrcPort >= 27000 and udp.SrcPort <= 27100"
Automatic Orbs dup for Warlock Well Of Radiance (just press F)

    %PathToExe% 12 0 900 70 "udp.SrcPort >= 27000 and udp.SrcPort <= 27100"
Automatic Orbs dup for Titan Bubble (just press F)

    %PathToExe% 12 0 500 70 "udp.SrcPort >= 27000 and udp.SrcPort <= 27100"