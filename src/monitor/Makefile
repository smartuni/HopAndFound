# name of your application
APPLICATION = HopAndFound_Monitor

# If no BOARD is found in the environment, use this default:
BOARD ?= pba-d-01-kw2x

DIRS += $(CURDIR)/../common

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT
# Uncomment this to enable scheduler statistics for ps:
#CFLAGS += -DSCHEDSTATISTICS
FEATURES_REQUIRED = periph_gpio periph_spi

# set default device parameters in case they are undefined
TEST_PCD8544_SPI   ?= SPI_0
TEST_PCD8544_CS    ?= GPIO_PIN\(2,4\)
TEST_PCD8544_RESET ?= GPIO_PIN\(3,4\)
TEST_PCD8544_MODE  ?= GPIO_PIN\(3,1\)

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
# export parameters
CFLAGS += -DTEST_PCD8544_SPI=$(TEST_PCD8544_SPI)
CFLAGS += -DTEST_PCD8544_CS=$(TEST_PCD8544_CS)
CFLAGS += -DTEST_PCD8544_RESET=$(TEST_PCD8544_RESET)
CFLAGS += -DTEST_PCD8544_MODE=$(TEST_PCD8544_MODE)
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
USEMODULE += pcd8544



#USEMODULE += shell
#USEMODULE += shell_commands
#USEMODULE += ps
#USEMODULE += gnrc_icmpv6_echo

include $(RIOTBASE)/Makefile.include

