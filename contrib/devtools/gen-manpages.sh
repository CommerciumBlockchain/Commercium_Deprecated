#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

COMMERCIUMD=${COMMERCIUMD:-$SRCDIR/commerciumd}
COMMERCIUMCLI=${COMMERCIUMCLI:-$SRCDIR/commercium-cli}
COMMERCIUMTX=${COMMERCIUMTX:-$SRCDIR/commercium-tx}
COMMERCIUMQT=${COMMERCIUMQT:-$SRCDIR/qt/commercium-qt}

[ ! -x $COMMERCIUMD ] && echo "$COMMERCIUMD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
CMMVER=($($COMMERCIUMCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for commerciumd if --version-string is not set,
# but has different outcomes for commercium-qt and commercium-cli.
echo "[COPYRIGHT]" > footer.h2m
$COMMERCIUMD --version | sed -n '1!p' >> footer.h2m

for cmd in $COMMERCIUMD $COMMERCIUMCLI $COMMERCIUMTX $COMMERCIUMQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${CMMVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${CMMVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
