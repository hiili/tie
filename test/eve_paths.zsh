export TIE_LIBDIR=$HOME/TIE/local

export VJ_BASE_DIR=/cavedata/jmlaitin/vrjuggler-1.0.7/instlinks
export FLUID_ROOT=/p/projects/fluid/current10
export MUSTAJUURI_PATH=/p/projects/tilmonen/mustajuuri-current2

export LD_LIBRARYN32_PATH=$VJ_BASE_DIR/lib32:$LD_LIBRARYN32_PATH
export LD_LIBRARYN32_PATH=$FLUID_ROOT/lib/IRIX-debug:$LD_LIBRARYN32_PATH
export LD_LIBRARYN32_PATH=$MUSTAJUURI_PATH/lib/IRIX-6.5:$LD_LIBRARYN32_PATH
export LD_LIBRARYN32_PATH=$TIE_LIBDIR/lib:$LD_LIBRARYN32_PATH
