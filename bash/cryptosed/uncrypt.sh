#!/bin/sh

ARCHIVE=$1

if [ -z "$ARCHIVE" ]; then
	ARCHIVE="tex2html-test.tar.7z"
fi

if [ ! -f "$ARCHIVE" ]; then
	echo 1>&2 "FFFFUUUUUUUUUUU"
	exit 1
fi

PASSWD=0
while [ ! -z "`7z x -y -p$PASSWD $ARCHIVE | grep 'Wrong password'`" ]; do
	PASSWD=`expr $PASSWD + 1`;

	if [ $PASSWD -lt 10 ]; then
		PASSWD="000$PASSWD"
	elif [ $PASSWD -lt 100 ]; then
		PASSWD="00$PASSWD"
	elif [ $PASSWD -lt 1000 ]; then
		PASSWD="0$PASSWD"
	fi

	if [ $PASSWD -gt 9999 ]; then
		echo "that's too difficult"
		exit 1
	fi

	echo "Trying password: $PASSWD"
done

echo "ok"
exit 0
