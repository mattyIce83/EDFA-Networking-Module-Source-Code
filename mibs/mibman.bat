rm -f EARMIBAction.c
smicng -z -cm EARMIB.inc > EARMIB.out
java -jar ../../../../bin/mibman.jar list
