# name of your application
APPLICATION = HopAndFound_Node

# If no BOARD is found in the environment, use this default:
BOARD ?= pba-d-01-kw2x

DIRS += $(CURDIR)/../common

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT
# Uncomment this to enable scheduler statistics for ps:
#CFLAGS += -DSCHEDSTATISTICS

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP -I$(CURDIR)/../include

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6
USEMODULE += gnrc_sock_check_reuse
USEMODULE += gnrc_sock_udp
USEMODULE += xtimer
USEMODULE += HAF-common

#USEMODULE += shell
#USEMODULE += shell_commands
#USEMODULE += ps
#USEMODULE += gnrc_icmpv6_echo

include $(RIOTBASE)/Makefile.include

