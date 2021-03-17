#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd $DIR/../../src

# get all headers and bring them in the correct format: '-i AAA.h -i BBB.h '
HEADER_PATHS=$(find -name "*.h*"  | awk '{print "-i " $1}')

# generate PlantUML file
mkdir -p $DIR/output
echo $HEADER_PATHS | xargs hpp2plantuml -o $DIR/output/output.puml -t $DIR/plantuml_template.puml

CUSTOM_LINES='
communication.communication::message_layer -down-->  communication.A1
communication.A1 -down--> communication.communication::byte_layer
hide communication.A1

communication.communication::connection_layer -down-->  communication.B1
communication.B1 -down--> communication.communication::message_layer
hide communication.B1
'

# encode newlines
CUSTOM_LINES="${CUSTOM_LINES//$'\n'/'\n'}"

sed -i "s@/' Inheritance relationships '/@""${CUSTOM_LINES}""\n/' Inheritance relationships '/@" $DIR/output/output.puml

# configure PlantUML
export PLANTUML_LIMIT_SIZE=16384

# run PlantUML to create png
java -jar $DIR/bin/plantuml.jar --verbose $DIR/output/output.puml

echo "UML done"
