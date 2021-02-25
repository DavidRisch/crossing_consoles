#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

mkdir -p $DIR/bin

# https://sourceforge.net/projects/plantuml/files/plantuml.jar/download
curl https://jztkft.dl.sourceforge.net/project/plantuml/plantuml.jar --output $DIR/bin/plantuml.jar

pip3 install hpp2plantuml
