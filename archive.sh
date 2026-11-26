#!/bin/sh

. ../../build.sh

TAR_FILE=sock_lib.${VERSION}_${BUILD}_${DATE}.tar

make clean

cd ..

tar cvf ${TAR_FILE} sock_lib

gzip ${TAR_FILE}

scp ${TAR_FILE}.gz apps.newio.org:~

