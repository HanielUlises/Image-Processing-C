CC = gcc
CFLAGS = -Wall -Wextra
SRCS = main.c imageBrightness.c
OBJDIR = objects
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
EXEC = brightness

all: $(OBJDIR) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(EXEC)