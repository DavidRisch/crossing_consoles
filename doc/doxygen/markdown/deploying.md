\page deploy-game Deploying the Game

This page contains instructions on how to deploy the game on a public server users can ssh into.

# Creating a Locked Down User

Create a user which people will later be given access to
(based on this [serverfault answer](https://serverfault.com/a/998437)):

    sudo useradd cc --shell /bin/rbash # use restricted bash
    sudo mkdir /home/cc
    sudo mkdir /home/cc/bin
    sudo touch /home/cc/.hushlogin # do not display motd
    # Add allowed binaries:
    sudo ln -s /usr/bin/clear /home/cc/bin/clear

Edit the `sshd_config` (`sudo vim /etc/ssh/sshd_config`):

    Match User cc
      PasswordAuthentication yes
      AllowTcpForwarding no
      X11Forwarding no
      AllowAgentForwarding no
      PermitTunnel no
      ForceCommand rbash --norc

Now restart the daemon: `sudo service ssh restart`.

Create the `.bashrc` (`sudo vim /home/cc/.bashrc`) with the following content:

    export PATH=/home/cc/bin
    
    # Needed for clear command to work
    export TERM=xterm-256color
    
    export PS1='$ '
    
    NORMAL='\e[39m'
    COMMAND='\e[31m'
    DEFAULT='\e[49m\e[39m'
    
    echo -e "
    ${NORMAL}
    Welcome!
    
    Type '${COMMAND}crossing_consoles [YOURNAME]${NORMAL}' to start the game (or the alias '${COMMAND}cc${NORMAL}').
    You can also set your color using hexadecimal codes: '${COMMAND}cc [YOURNAME] #aa2211${NORMAL}'${DEFAULT}"

# Installing the Game Client

The crossing consoles client binary can now be placed in the `/home/cc/bin` directory. \
Alternatively a simlink can be created:

    sudo ln -s /path/to/crossing_consoles_main_client /home/cc/bin/crossing_consoles
    sudo ln -s /home/cc/bin/crossing_consoles /home/cc/bin/cc

# Installing the Game Server

Create and run a script to start the server every time it exits
(based on this [stackoverflow answer](https://stackoverflow.com/a/697064/13623303)):

    SERVER_BIN=/path/to/crossing_consoles_main_server
    
    until $SERVER_BIN; do
        echo "Server exited with exit code $?.  Respawning..." >&2
        sleep 0.1
    done
