#!/bin/bash


STATUS="Generating data ..."
echo $STATUS

#size in bytes
COALESCE_FILE_SIZE="5000000"

#how many rounds of random chunkcs
RANDOM_CHUNKS_ROUNDS=5

#chunk size in kilobytes
CHUNK_SIZE=50

WIKIPEDIA_HAR="winter-war-en.wikipedia.org.har"
WIKIPEDIA_HAR_DOCS="winter-war-en.wikipedia.org-docs"
WIKIPEDIA_COALESCE="wikipedia-coalesce-output"
WIKIPEDIA_RANDOM_CHUNKS="wikipedia-random-chunks"

rm -rf $WIKIPEDIA_RANDOM_CHUNKS
mkdir $WIKIPEDIA_RANDOM_CHUNKS

python2.7 ../datacomp_utils/har2docs.py --stream $WIKIPEDIA_HAR $WIKIPEDIA_HAR_DOCS  wikipedia
python coalesce.py $WIKIPEDIA_HAR_DOCS $WIKIPEDIA_COALESCE $COALESCE_FILE_SIZE

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

