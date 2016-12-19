APPLICATION = display

# If no BOARD is found in the environment, use this default:
BOARD ?= pba-d-01-kw2x

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

FEATURES_REQUIRED = periph_gpio periph_spi

USEMODULE += shell
USEMODULE += pcd8544

# set default device parameters in case they are undefined
TEST_PCD8544_SPI   ?= SPI_0
TEST_PCD8544_CS    ?= GPIO_PIN\(2,4\)
TEST_PCD8544_RESET ?= GPIO_PIN\(3,4\)
TEST_PCD8544_MODE  ?= GPIO_PIN\(3,1\)

# export parameters
CFLAGS += -DTEST_PCD8544_SPI=$(TEST_PCD8544_SPI)
CFLAGS += -DTEST_PCD8544_CS=$(TEST_PCD8544_CS)
CFLAGS += -DTEST_PCD8544_RESET=$(TEST_PCD8544_RESET)
CFLAGS += -DTEST_PCD8544_MODE=$(TEST_PCD8544_MODE)

include $(RIOTBASE)/Makefile.include
