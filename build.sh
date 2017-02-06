./boot.sh
./configure --with-linux=/lib/modules/`uname -r`/build
make -j4
