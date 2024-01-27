CC = gcc

CFLAGS = -Wall -Wextra

SRCS = main.c copy.c
OBJDIR = objects

OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

EXEC = copyImage

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(EXEC)
