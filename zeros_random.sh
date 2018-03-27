
INPUT_DIR=$1

if [ -z "$1" ]
then
  echo "bad argument"
  exit 555
fi

ZEROS_INPUT_DIR=$INPUT_DIR/zeros
RANDOM_INPUT_DIR=$INPUT_DIR/random

rm -rf $ZEROS_INPUT_DIR
rm -rf $RANDOM_INPUT_DIR

mkdir $ZEROS_INPUT_DIR
mkdir $RANDOM_INPUT_DIR

dd if=/dev/urandom of=$RANDOM_INPUT_DIR/1 count=500 bs=1MB
dd if=/dev/urandom of=$RANDOM_INPUT_DIR/2 count=500 bs=1MB
dd if=/dev/urandom of=$RANDOM_INPUT_DIR/3 count=500 bs=1MB

dd if=/dev/zero of=$ZEROS_INPUT_DIR/1 count=500 bs=1MB
dd if=/dev/zero of=$ZEROS_INPUT_DIR/2 count=500 bs=1MB
dd if=/dev/zero of=$ZEROS_INPUT_DIR/3 count=500 bs=1MB



