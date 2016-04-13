# ProcPatch:    

Fixes UnrealEd v3369 procedural sound playback and properties.

## Usage:        
  Put it in /UT2004/System/ folder and run.
  
## Expert:       
File offset can be specified, ie ProcPatch.exe 0x16db2f


## Additional Notes:

- UT2004 v3369 only.
- Backup your UnrealEd.exe first.
- After patching Procedural Sounds won't be displayed with ">" prefix anymore.
- This is unofficial and not really tested patch -- better backup your stuff. 


## How does it work:

- Sound browser prefixes procedural sounds with ">" and sound groups with "*".
- In procedural sounds case the prefix apparently isn't stripped before passing the sound name to other functions.
- This patch fixes the bug by removing the ">" prefix.


## UED Sound Browser bug workaround:

- By default when you open UED and select ie "WeaponSounds" package, all sounds  are listed but the "All" button isn't pressed. 
- In this case Sound Browser may not display Properties window if the sound is located in some package group.
- To fix, click on the "All" button or select some group.
