#!/bin/bash


awk < latex/refman.tex >__latex_postproc_tempfile__ \
  '{ if( NR == 2 ) print "\\usepackage[finnish]{babel}" ; print $0 }'
mv __latex_postproc_tempfile__ latex/refman.tex

for file in latex/*.tex ; do
  sed -i 's/: k.*n opas//' $file
  sed -i 's/The Incredible Environment Page Documentation/Käyttäjän opas/' $file
done

sed -i 's/The Incredible Environment Reference Manual/The Incredible Environment: käyttäjän opas/' latex/refman.tex


echo done
echo
