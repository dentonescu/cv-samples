#!/bin/sh
SCRIPT_DIR="$(cd -- "$(dirname -- "$0")" && pwd)"
h2_jar="$(find ~/.gradle -name "h2*[0-9].jar" | head -n 1)"
if [ -z "$h2_jar" ]; then
 echo "ERROR: Failed to find the h2 jar"
 exit 1
fi
db_loc="$(realpath "$SCRIPT_DIR/../data/heapdb")"
java -cp "$h2_jar" org.h2.tools.Shell -url "jdbc:h2:file:$db_loc;DB_CLOSE_ON_EXIT=FALSE;AUTO_RECONNECT=TRUE" -user sa
