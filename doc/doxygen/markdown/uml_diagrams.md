\page uml-diagrams Software Architecture

The software architecture is divided in multiple layers:

* \subpage uml-byte
* \subpage uml-messages
* \subpage uml-connection
* \subpage uml-game

UML Diagrams showing all relations between the classes of one namespace are provided here: \subpage uml-src.  
All shown UML Diagrams can be generated using the scripts provided in the `crossing_consoles/doc/uml/` directory:

    git clone git@github.com:DavidRisch/crossing_consoles.git
    cd crossing_consoles/doc/uml/
    ./install.sh

Run the script using Python3:

    python3 build.py
