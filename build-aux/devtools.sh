dname="`dirname $0`"
if [ "$dname" = "." ]; then
  export PATH="$PWD:$PATH"
  export MILENA_DEVTOOLS_PATH="$PWD"
elif [ -d "$PWD/$dname" ]; then
  export PATH="$PWD/$MILENA_DEVTOOLS_PATH:$PATH"
  export MILENA_DEVTOOLS_PATH="$PWD/$dname"
else
  echo "Cannot guess Milena's devtools path. Please set MILENA_DEVTOOLS_PATH variable manually." 
fi
