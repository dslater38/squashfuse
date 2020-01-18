# SQUASHFS-Win #
## Squash filesystem for Windows ##

### Basic Usage with windows network redirector: ###
Map a squash file system archive as a network drive:  
`squashfs` is the server name. The path is the full path to 
your squash archive minus the `:`.    
For example, suppose I have an archive:   
`C:\mysysroots\arm-linux-gnueabihf.sqf`.    
I can mount it on drive `X:` by mapping `X:` to:    
`\\squashfs\C\mysysroots\arm-linux-gnueabihf.sqf`

![map network drive image][mapnetworkdrive]

Or from a command line prompt:

`> net use x: \\squashfs\C\mysysroots\arm-linux-gnueabihf.sqf`

### Command line tools: ###


[mapnetworkdrive]: ./images/MapNetworkDrive.jpg "Map Network Drive"
