#!/bin/bash


STATUS="Generating data ..."
echo $STATUS

COALESCE_FILE_SIZE="1000000"

WIKIPEDIA_HAR="winter-war-en.wikipedia.org.har"
WIKIPEDIA_HAR_DOCS="winter-war-en.wikipedia.org-docs"
WIKIPEDIA_COALESCE="coalesce-output"


python2.7 ../datacomp_utils/har2docs.py --stream $WIKIPEDIA_HAR $WIKIPEDIA_HAR_DOCS  wikipedia
python coalesce.py $WIKIPEDIA_HAR_DOCS $WIKIPEDIA_COALESCE $COALESCE_FILE_SIZE