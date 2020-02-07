# Target library
lib := libuthread.a

objs    := preempt.o context.o uthread.o queue.o

all: $(lib)

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror
CFLAGS  += -g

ifneq ($(V),1)
Q = @
endif

%.o: %.c %.h
	@echo "CC   $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

$(lib): $(objs)
	ar rcs $@ $^

clean:
	@echo "CLEAN   $@"
	$(Q)rm -f $(objs) *.a
