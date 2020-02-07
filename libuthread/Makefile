# Target library
lib 	:= libuthread.a
targets	:= $(lib)
objs	:= queue.o uthread.o context.o preempt.o

CC	:= gcc
CFLAGS	:= -Wall -Werror 
CFLAGS  += -g



all: $(targets)

deps	:= $(patsubt %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

$(lib): $(objs)
	@echo "make lib"
	$(Q)ar rcs $(lib) $(targets)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

clean:
	@echo "CLEAN"
	$(Q)rm -f $(targets) $(objs) $(deps) *.d

