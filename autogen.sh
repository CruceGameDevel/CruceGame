#!/bin/sh

mkdir m4
if [ ! -d "m4" ]; then
  echo "Unable to create m4 directory";
  echo "exiting the program";
  echo "create m4 directory manually here in current path";
  exit;
fi

run()
{
    $@
    if test $? -ne 0; then
        echo "Failed $@"
        exit 1
    fi
}

run aclocal ${ACLOCAL_ARGS}
run libtoolize --copy --force
run autoheader
run automake --add-missing --foreign --copy
run autoconf
