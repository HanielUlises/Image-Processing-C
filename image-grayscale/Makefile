CC = gcc
CFLAGS = -Wall -Wextra
SRCS = main.c imageGrayscale.c
OBJDIR = objects
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
EXEC = toGray

all: $(OBJDIR) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(EXEC)
