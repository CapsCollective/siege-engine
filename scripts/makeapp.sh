#!/bin/bash

# Require three arguments for app name, contents directory path, startup script and build directory
if [ $# -ne 4 ]; then
  echo "Requires four arguments (e.g. makeapp <app_name> <contents_dir> <startup_script> <build_dir>)"
  exit 1
fi

# Set app details
APP_NAME=$1
BUILD_DIR=$4
APP_FILE="$BUILD_DIR/$APP_NAME.app"
APP_CONTENTS=$2
CONTENTS_DIR="Contents"
APP_SCRIPT=$3

# Create the app file with run logic
mkdir $APP_FILE
echo "#!/bin/bash
abspath () 
{ 
case \"\${1}\" in
    [./]*)
    local ABSPATH=\"\$(cd \${1%/*}; pwd)/\"
    echo \"\${ABSPATH/\/\///}\"
    ;;
    *)
    echo \"\${PWD}/\"
    ;;
esac
}
LOCAL_PATH=\`abspath \${0}\`
cd \$LOCAL_PATH/$CONTENTS_DIR
./$APP_SCRIPT" > $APP_FILE/$APP_NAME
chmod +x $APP_FILE/$APP_NAME

# Add app contents
mkdir $APP_FILE/$CONTENTS_DIR
cp -r $APP_CONTENTS/* $APP_FILE/$CONTENTS_DIR
