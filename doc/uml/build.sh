#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd $DIR/../../src

# get all headers and bring them in the correct format: '-i AAA.h -i BBB.h '
HEADER_PATHS=$(find -name "*.h*"  | awk '{print "-i " $1}')

# generate PlantUML file
mkdir -p $DIR/output
echo $HEADER_PATHS | xargs hpp2plantuml -o $DIR/output/output.puml -t $DIR/plantuml_template.puml

# configure PlantUML
export PLANTUML_LIMIT_SIZE=16384

# run PlantUML to create png
java -jar $DIR/bin/plantuml.jar --verbose $DIR/output/output.puml

echo "UML done"
