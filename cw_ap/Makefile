
CC=gcc
VPATH = ../common
LDFLAGS = 
CFLAGS = -Wall -O0 -I../common
CFLAGS += -g

RM=/bin/rm -f

WTP_OBJ = wtp.o wtp_discovery.o \
           cw_common.o cw_protocol.o cw_network.o \
           cw_util.o cw_log.o cw_thread.o

WTP_SRC = $(WTP_OBJ:.o=.c)
WTP_NAME = wtp

.PHONY: deps clean libs

all: $(WTP_NAME)

$(WTP_NAME): $(WTP_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

deps: $(WTP_SRC)
	$(CC) $(WTP_SRC) $(CFLAGS)

clean:
	$(RM) $(WTP_OBJ)

