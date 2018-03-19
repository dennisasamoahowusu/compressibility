#!/bin/bash

#values are wiki, fb, yt, rn, ze
DATA_TYPE=$1


if [ -z "$DATA_TYPE" ]  
then
  echo "Bad data type: use ./generate_data.sh <data-type>"
  exit 	777
fi 

if [ "$DATA_TYPE" != 'wiki' ] && [ "$DATA_TYPE" != 'fb' ] && [ "$DATA_TYPE" != 'yt' ] && [ "$DATA_TYPE" != 'rn' ] && [ "$DATA_TYPE" != 'ze' ] 
then
  echo "Bad data type: use ./generate_data.sh <data-type>"
  exit 777
fi 

STATUS="Generating data ..."
echo $STATUS

#size in bytes
COALESCE_FILE_SIZE="500000"

#how many rounds of random chunkcs
RANDOM_CHUNKS_ROUNDS=5

#chunk size in kilobytes
CHUNK_SIZE=50


################################# Setting files and folders ####################################################
HAR_DIRECTORY="/home/dennis/coding/compressibility_data/compressibility_data"

WIKIPEDIA_OUTPUT="wiki_output"
WIKIPEDIA_HAR="$HAR_DIRECTORY/wikipedia.har"

FACEBOOK_OUTPUT="fb_output"
FACEBOOK_HAR="$HAR_DIRECTORY/facebook.har"

YOUTUBE_OUTPUT="yt_output"
YOUTUBE_HAR="$HAR_DIRECTORY/youtube.har"

ZEROS_OUTPUT="ze_output"

RANDOM_OUTPUT="rn_output"


# set output and har based on data type
if [ $DATA_TYPE == 'wiki' ]
then
  OUTPUT=$WIKIPEDIA_OUTPUT
  HAR=$WIKIPEDIA_HAR
elif [ $DATA_TYPE == 'fb' ]
then
  OUTPUT=$FACEBOOK_OUTPUT
  HAR=$FACEBOOK_HAR
elif [ $DATA_TYPE == 'yt' ]
then
  OUTPUT=$YOUTUBE_OUTPUT
  HAR=$YOUTUBE_HAR
elif [ $DATA_TYPE == 'rn' ]
then
  OUTPUT=$RANDOM_OUTPUT
elif [ $DATA_TYPE == 'ze' ]
then
  OUTPUT=$ZEROS_OUTPUT
else
  exit 444
fi

HAR_DOCS="$OUTPUT/har-docs"
COALESCE="$OUTPUT/coalesce"
RANDOM_CHUNKS="$OUTPUT/random-chunks"


echo "############################################ $DATA_TYPE ########################################"
echo "Generating data for $DATA_TYPE ....."

echo "Deleting $OUTPUT dir ....."
rm -rf $OUTPUT

echo "Creating $OUTPUT dir ....."
mkdir $OUTPUT


if [ "$DATA_TYPE" != 'rn' ] && [ "$DATA_TYPE" != 'ze' ] 
then
  echo "Converting har to docs ....."
  python2.7 ../datacomp_utils/har2docs.py --stream $HAR $HAR_DOCS $DATA_TYPE

  echo "Coalescing docs to larger files ......"
  python coalesce.py $HAR_DOCS $COALESCE $COALESCE_FILE_SIZE
fi 


if [ $DATA_TYPE == 'rn' ]
then
  FILES=$HAR_DIRECTORY/random/*
elif [ $DATA_TYPE == 'ze' ]
then
  FILES=$HAR_DIRECTORY/zeros/*
else
  FILES=$COALESCE/*
fi




echo "Creating random chunks from larger files"
mkdir $RANDOM_CHUNKS
COUNTER=1
for f in $FILES
do
  for k in `seq 1 $RANDOM_CHUNKS_ROUNDS`;
    do
      ../randomchunk/randomchunk -i $f -o $RANDOM_CHUNKS/$COUNTER -s $CHUNK_SIZE
      let COUNTER=COUNTER+1
    done 
done

# bytecounting
echo "Bytecounting ......"
python ecetools.py bc $RANDOM_CHUNKS $OUTPUT/bc_ece_output

echo "Byte stdev ......"
#byte standard deviation 
python ecetools.py bs $RANDOM_CHUNKS $OUTPUT/bs_ece_output
