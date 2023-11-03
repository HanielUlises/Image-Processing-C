CC = gcc
CFLAGS = -Wall -Wextra
SRCS = main.c binarization.c
OBJDIR = objects
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
EXEC = binarize

all: $(OBJDIR) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(EXEC)