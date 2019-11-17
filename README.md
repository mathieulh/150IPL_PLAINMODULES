# 150IPL_PLAINMODULES
This ipl hacks the 1.50 ipl to allow it to run decrypted payload, decrypted prx's and txt config files.


Binary installation: 

- Flash the ipl_result.bin using the ipl flasher.
- Flash the files loadcore.prx and modulemgr.prx, that are pre-patched for noplainmodulecheck
  and no-kxploit respectively.

Pandora IPL block Compilation instructions:

- Compile iplboot
- Compile patch_main
- Move both to build_ipl folder.
- Get the part1 of the 1.50 ipl and move it also there with the name ipl.bin.
- Get the part3 (payload) of the 1.50 ipl, move it there with the name payload.bin and patch it accordingly to Report_IPLPayload.txt
or by using the payload.ips IPS patch file. These patches will make the IPL load a decrypted sysmem and loadcore.
- Execute build_ipl and it will generate it.
- Get the decrypted loadcore.prx and modulemgr.prx modules from 1.50 and patch those using their respective ips files or manually using
the Report_loadcore.txt and Report_modulemgr.txt files


Proper IPL block instructions (works on DTP-T1000):

- Get the 1.50 ipl and decrypt it.
- Get the part3 (payload) of the 1.50 ipl, move it there with the name payload.bin and patch it accordingly to Report_IPLPayload.txt
or by using the payload.ips IPS patch file. These patches will make the IPL load a decrypted sysmem and loadcore.
- Reencrypt the part 3 IPL block using ipltool (https://github.com/zecoxao/ipltool) and the following parameters (set the data and block size accordingly to your payload)

<pre>
unsigned long int entry			= 0x00000000;
u32 loadAddr 					= 0x00000000;
</pre>

- Replace the payload IPL block in the decrypted IPL and reencrypt it with ipltool using default values
- Get the decrypted loadcore.prx and modulemgr.prx modules from 1.50 (or the decrypted development tool versions) and patch those using their respective ips files or manually using
the Report_loadcore.txt and Report_modulemgr.txt files

<pre>
Patched loadcore.prx: SHA-256: 453B8E0C74317FC483BC3BE31DF47B641B3470E8D8932D517EA75D02DA1D7125
Patched modulemgr.prx: SHA-256: 65267351AFE51F2B0E3F52FC6218C9431159B6B98EE85835240C2C9216FEA64B
Patched loadcore.prx for tool: SHA-256: 5F5CB7F65BA81A10D65F548E7773E82567598AD4DFC7834B1E1B59A63AEACBCF
Patched modulemgr for tool: SHA-256: 7BEC0AB4D88DFFDBDA93639AB8FC7EBA2B3248A835C0A99692212E14061CC5DC
patched payload.bin: SHA-256: 12DED8D46E1BEF4ECAEAE1B862F858238369DCBBE3D9374DAAE57772EBB101FC
</pre>
