#!/bin/bash
. `dirname $0`/functions.sh
# This script copies needed C and C++ libraries into the test directory
echo 'int main() { }' > movelibs.C
$RUN_HOST $CXX -o movelibs movelibs.C
> syslib.list
> syslnk.list
for i in `$RUN_HOST RTLD_TRACE_PRELINKING=1 RTLD_WARN= $LDD ./movelibs \
	  | awk '$1 !~ /^\.\/movelibs/ { print $3 } '`; do
  k=`basename $i`
  if [ -L $i ]; then
    j=`ls -l $i | sed 's/^.* -> //'`
    if echo $j | grep / >/dev/null 2>&1; then
      cp -p $i .
      echo $k >> syslib.list
    else
      cp -dp $i .
      cp -p `dirname $i`/$j .
      echo $j >> syslib.list
      echo $k >> syslnk.list
    fi
  else
    cp -p $i .
    echo $k >> syslib.list
  fi
done
rm -f movelibs.C movelibs
pwd > prelink.conf
for i in `cat syslib.list`; do
  if $RUN_HOST $READELF -WS $i 2>/dev/null | grep -q .gnu.prelink_undo; then
    $RUN_HOST $PRELINK -u $i > /dev/null 2>&1 || exit 1
  fi
  cp -p $i $i.orig
done
exit 77
