#
# $Id: makefile,v 1.4 2011-10-11 13:46:40 hazem Exp $
#
# $Log: makefile,v $
# Revision 1.4  2011-10-11 13:46:40  hazem
# This version adds 15Min Trap meassage and Allow to remote reset the
# system via CLI and SNMP
#
# Revision 1.3  2011/06/02 17:47:18  hazem
# Files modified:  Added to display a "*" when link is up on the management
# port and fixed set timeout bug.  This is version 1.143
#
# M lib/32b/libbsp.a
# M src/apps/ipg/EARMIB.c
# M src/apps/ipg/earRtcTime.c
# M src/apps/ipg/menu.c
# M src/apps/ipg/root.c
# M src/apps/ipg/32b/image.bin
# M src/apps/ipg/32b/image.elf
# M src/apps/ipg/32b/image.map
# M src/apps/ipg/32b/image.sym
# M src/apps/ipg/32b/image.uncompressed
# M src/apps/ipg/32b/makefile
# M src/apps/ipg/32b/rom.bin
# M src/apps/ipg/32b/rom.elf
# M src/apps/ipg/32b/rom.map
# M src/apps/ipg/32b/rom.sym
# M src/apps/ipg/32b/version.bin
# M src/apps/ipg/mibs/EARMIB.smi
# M src/bsp/depend.mk
# M src/bsp/bootloader/libs/libnet.a
# M src/bsp/platforms/connectme/MII.c
#
# Revision 1.2  2010/03/08 20:13:26  eugene
# Bug_908: Changed version to 1.141.
#
#
#
# All application Makefiles should define the following macros:
#
#   NETOS_DIR       :  relative path to the "netos" toplevel
#   BUILD_RAM_IMAGE :  set to "0" if not desired, and to "1"
#                      if desired (and supported by platform)
#   BUILD_ROM_IMAGE :  set to "0" if not desired, and to "1"
#                      if desired (and supported by platform)
#   APP_C_FILES     :  list of application specific "C" files
#                      (must all have a ".c" suffix)
#   APP_C_FLAGS     :  application specific CFLAGS
#   APP_CC_FILES    :  list of application specific "C++" files
#                      (must all have either a ".cc", ".C", or ".cxx" suffix)
#   APP_CC_FLAGS    :  application specific CFLAGS for C++ compilation
#   APP_ASM_FILES   :  list of application specific assembly files
#                      (must all have either a ".s", ".S", or ".arm" suffix)
#   APP_ASM_FLAGS   :  application specific ASFLAGS for assembly
#   APP_INCDIRS     :  list of application specific include directories
#                      in "-I.." format.
#
#   APP_DESCRIPTION :  Simple one line description of application
#                      (for applicable platforms, will be incorporated
#                      into the binary VPD)
#
#   USE_SIMPLE_WEB_SERVER : set to "1" if desired (used for compatibility
#                           with older examples... links against a different
#                           set of libraries)
#
#   INCLUDE_CXX_STARTUP : set to "1" to include C++ initialization libraries
#
#   APP_OBJ_FILES   :  An optional veriable which can specify "extra"
#                      objects to be linked into an image which are
#                      not derived from c, c++, or assembly.  The
#                      application Makefile bears the burden of providing
#                      rules to build these files if necessary.
#
# The application Makefile should also include VPATH specifications
# for all application source.
#

NETOS_DIR= ../../../..

BUILD_RAM_IMAGE= 1
BUILD_ROM_IMAGE= 1

APP_DESCRIPTION="IPG Photonics SNMP Application"

MAJOR_VERSION= 1
# was 149 Fixed Timeout issue
MINOR_VERSION= 194

vpath %.c ..

#  Added telnet.c and menu.c for telnet capabilities - 05/25/05

APP_C_FILES=root.c \
        MibMan.c \
        EARMIB.c \
        EARMIBAction.c \
	ipgFlash.c \
	telnet.c \
	menu.c \
	ipgsnmp.c \
	earTemperatureAlarmThreshold.c \
	earSnmpMngrTable.c \
	earTrapPort.c \
	earPumpsOff.c \
	earChassisReboot.c \
	earModelNumberRead.c \
	earRtcTime.c \
	find_highwater.c \
	readFrom2U2.c \
	semaphores.c \
	Snmp_api.c \
	configNotification.c \
	atmel.c \
	EDFAWelcomePage.c \
	EDFAWelcomePage_v.c \
	RpPages.c \
	security.c \
	RpUsrDct.c \
	cgi.c \
	file.c \
	naftpapp.c

APP_CC_FILES=
APP_ASM_FILES=

APP_C_FLAGS= -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION)
APP_CC_FLAGS= 
APP_ASM_FLAGS=

APP_INCDIRS= -I..

# put the version information into the elf file header
EXTRA_HEADER= version.bin
$(EXTRA_HEADER): makefile
	echo $(EXTRA_HEADER) "contains" $(MAJOR_VERSION)"."$(MINOR_VERSION)
	verstmp $(EXTRA_HEADER) $(MAJOR_VERSION) $(MINOR_VERSION)

#
# All "required" elements must be supplied prior to including the
# "core" Makefile and targets.
#

include $(NETOS_DIR)/src/linkerScripts/Makefile.app.inc

#
# Application specific macros and targets can go below here.
#

all:
	#$(RM) -f $(EXTRA_HEADER)
	od -t x2 -j 36 -N 4 image.bin 

dump_ver: image.bin
	od -t x2 -j 36 -N 4 $? 

