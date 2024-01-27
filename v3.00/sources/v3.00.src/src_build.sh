#!/bin/bash

THIS_DIR=`pwd`

OS=`uname -s`

export OS
export CC
export EXTRA_CFLAGS
export BINUTILS_CONFIGURE
export EXE

case $OS in
  Darwin)  OS="OS/X"
             CC="gcc"
             EXTRA_CFLAGS=""
             BINUTILS_CONFIGURE="--with-included-gettext --target=pic30-@omf32 --host=x86_64-darwin"
             EXE=""
             ;;
  CYGWIN*) OS="Windows/Cygwin"
             CC="i686-pc-mingw64-gcc"
             EXTRA_CFLAGS=""
             BINUTILS_CONFIGURE="--with-included-gettext --target=pic30-@omf32 --host=x86_64-pc-mingw32"
             EXE=".exe"
             ;;
  *)       OS="Linux/Unknown"
             CC="gcc"
             EXTRA_CFLAGS=""
             BINUTILS_CONFIGURE="--with-included-gettext --target=pic30-@omf32 --host=x86_64-linux"
             EXE=""
             ;;
esac


echo "Building for $OS"

mkdir -p install/bin/bin
export install_dir=`pwd`/install

mkdir bin
(
  cd bin

  for OMF in elf; do
    mkdir binutils-${OMF}-native
    ( 
       cd binutils-${OMF}-native
       lconfigure=`echo $BINUTILS_CONFIGURE | sed -e 's/@omf/${OMF}/g'`
       lconfigure=`eval echo $lconfigure`
       CFLAGS="-DPIC30ELF -DPIC30 -DRESOURCE_MISMATCH_OK" $THIS_DIR/src/binutils-2.32/configure $lconfigure
       make
       cp -f gas/as-new${EXE} ${install_dir}/bin/bin/${OMF}-as${EXE}
       cp -f ld/ld-new${EXE}  ${install_dir}/bin/bin/${OMF}-ld${EXE}
       cp -f binutils/objdump${EXE} ${install_dir}/bin/bin/${OMF}-objdump${EXE}
       cp -f binutils/bin2hex${EXE} ${install_dir}/bin/bin/${OMF}-bin2hex${EXE}
       cp -f binutils/strip-new${EXE} ${install_dir}/bin/bin/${OMF}-strip${EXE}
       cp -f binutils/ar${EXE}        ${install_dir}/bin/bin/${OMF}-ar${EXE}
    )

    SRC=`ls -1d $THIS_DIR/src/xc16*`
    mkdir gcc-${OMF}-native
    (
       cd gcc-${OMF}-native
       export EXTRA_CFLAGS
       $THIS_DIR/build_XC16_831 -install_dir=${install_dir} -src=$SRC -omf=$OMF -cross=native64-nolm  -DMCHP_VERSION=v0_00
       cp gcc/gcc/xgcc${EXE} ${install_dir}/bin/bin/${OMF}-gcc${EXE}
       cp gcc/gcc/cc1${EXE} ${install_dir}/bin/bin/${OMF}-cc1${EXE}
       cp gcc/gcc/cpp${EXE} ${install_dir}/bin/bin/${OMF}-cpp${EXE}
    )
  done

)

cp src/c30_resource/src/c30/c30_device.info $install_dir/bin/

