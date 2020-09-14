# ZippySolver 

An open source solver developed by the Zenith Poker community.

# Installation

The instructions here are for Windows 10.

1.  Install the Windows Subsystem for Linux:

    Run the following command in Windows Powershell:

    ```powershell
    Enable-WindowsOptionalFeature -Online -FeatureName Microsoft-Windows-Subsystem-Linux
    ```
	This will require a restart.
	
2.  Install Ubuntu Linux from the Windows Store. Currently this is version
    20.04::

    Open up the Windows Store. Search for Ubuntu. Open up Ubuntu and press "Get"
    to install this.

3.  First time run of Ubuntu:

    Click on the Start Button and choose the Ubuntu icon. Wait until the distro installs. 
	Provide a username and password for the default user account. 
	
	Note that this account is a member of the Linux administrators (sudo) group so choose a secure username and password combination.

4.  Update / Upgrade packages (optional step)

    ```bash
    sudo apt-get update
    sudo apt-get upgrade
    ```

5.  Install ZippySolver through git:

    ```bash
    git clone https://github.com/ZenithPoker/ZippySolver
    cd ZippySolver/ZippyEngine
    ```
	
6.	Build whatever binaries you need from the top level. For example:

	```
	make bin/build_hand_value_tree
	```

7.	TODO
