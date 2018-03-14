#!/bin/bash


STATUS="Generating data ..."
echo $STATUS

#size in bytes
COALESCE_FILE_SIZE="5000000"

#how many rounds of random chunkcs
RANDOM_CHUNKS_ROUNDS=5

WIKIPEDIA_HAR="winter-war-en.wikipedia.org.har"
WIKIPEDIA_HAR_DOCS="winter-war-en.wikipedia.org-docs"
WIKIPEDIA_COALESCE="coalesce-output"


python2.7 ../datacomp_utils/har2docs.py --stream $WIKIPEDIA_HAR $WIKIPEDIA_HAR_DOCS  wikipedia
python coalesce.py $WIKIPEDIA_HAR_DOCS $WIKIPEDIA_COALESCE $COALESCE_FILE_SIZE

FILES=$WIKIPEDIA_COALESCE/*
for f in $FILES
do
  for i in `seq 1 $RANDOM_CHUNKS_ROUNDS`;
    do
      echo $f
    done 
done

