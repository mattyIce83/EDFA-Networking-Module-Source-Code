# $Id: makefile,v 1.1.1.1 2006-11-27 22:34:48 eugene Exp $

# Turn EARMIB.smi into something we can feed into the compiler
# Don't forget to set SMICAUTH envionment variable beforehand
# Tuned for EARMIB, though

#.out.inc:
#.inc.out:
#	smicng -z -cm $< > $@

ENDSRC = EARMIB.c EARMIB.h 
MIBSRC = MibMan.c MibMan.h
IPGREG = ipgReg.inc ipgReg.sm2 
RFCOUT = rfc1902.out rfc1903.out rfc1903.out rfc1904.out

EARMIBAction.c $(ENDSRC) $(MIBSRC): EARMIB.config EARMIB.smi $(IPGREG) EARMIB.out list 
	rm -f EARMIBAction.c
	java -jar ../../../../bin/mibman.jar list

split:
	gawk -f split_Action.awk EARMIBAction.c

EARMIB.out: EARMIB.smi EARMIB.inc $(RFCOUT)


clean:
	rm -f EARMIBAction.c 
	rm -f *.out


.SUFFIXES:  .sm2 .inc .out

.sm2.inc:
	touch $@

.inc.out:
	smicng -z -cm $< > $@


