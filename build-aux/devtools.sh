export MILENA_DEVTOOLS_PATH=`dirname $0`

if [ -d "$PWD/$MILENA_DEVTOOLS_PATH" ]; then
  export PATH=$PWD/$MILENA_DEVTOOLS_PATH:$PATH
elif [ -d "$MILENA_DEVTOOLS_PATH" ] && [ -z "`echo $MILENA_DEVTOOLS_PATH | cut -d '/' -f 1`" ]; then
  export PATH=$MILENA_DEVTOOLS_PATH:$PATH
else
  echo "Cannot guess Milena's devtools path. Please set MILENA_DEVTOOLS_PATH variable manually." 
fi
