#!/bin/bash


STRING="Generating data ..."
echo $STRING

python2.7 ../datacomp_utils/har2docs.py --stream winter-war-en.wikipedia.org.har winter-war-en.wikipedia.org-docs wikipedia
python coalesce.py winter-war-en.wikipedia.org-docs/ coalesce-output/ 200000