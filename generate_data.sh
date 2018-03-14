#!/bin/bash


STATUS="Generating data ..."
echo $STATUS

#size in bytes
COALESCE_FILE_SIZE="500000"

#how many rounds of random chunkcs
RANDOM_CHUNKS_ROUNDS=5

#chunk size in kilobytes
CHUNK_SIZE=50


############################################ WIKIPEDIA ########################################
echo "############################################ WIKIPEDIA ########################################"
echo "Generating data for wikipedia ....."

WIKIPEDIA_OUTPUT="wiki_output"

echo "Deleting $WIKIPEDIA_OUTPUT dir ....."
rm -rf $WIKIPEDIA_OUTPUT

echo "Creating $WIKIPEDIA_OUTPUT dir ....."
mkdir $WIKIPEDIA_OUTPUT

WIKIPEDIA_HAR="winter-war-en.wikipedia.org.har"
WIKIPEDIA_HAR_DOCS="$WIKIPEDIA_OUTPUT/har-docs"
WIKIPEDIA_COALESCE="$WIKIPEDIA_OUTPUT/coalesce"
WIKIPEDIA_RANDOM_CHUNKS="$WIKIPEDIA_OUTPUT/random-chunks"

echo "Converting har to docs ....."
python2.7 ../datacomp_utils/har2docs.py --stream $WIKIPEDIA_HAR $WIKIPEDIA_HAR_DOCS  wikipedia

echo "Coalescing docs to larger files ......"
python coalesce.py $WIKIPEDIA_HAR_DOCS $WIKIPEDIA_COALESCE $COALESCE_FILE_SIZE

echo "Creating random chunks from larger files"
mkdir $WIKIPEDIA_RANDOM_CHUNKS
COUNTER=1
FILES=$WIKIPEDIA_COALESCE/*
for f in $FILES
do
  for k in `seq 1 $RANDOM_CHUNKS_ROUNDS`;
    do
      ../randomchunk/randomchunk -i $f -o $WIKIPEDIA_RANDOM_CHUNKS/$COUNTER -s $CHUNK_SIZE
      let COUNTER=COUNTER+1
    done 
done

# bytecounting
echo "Bytecounting ......"
python ecetools.py bc $WIKIPEDIA_RANDOM_CHUNKS $WIKIPEDIA_OUTPUT/wiki_bc_ece_output

echo "Byte stdev ......"
#byte standard deviation 
python ecetools.py bs $WIKIPEDIA_RANDOM_CHUNKS $WIKIPEDIA_OUTPUT/wiki_bs_ece_output
