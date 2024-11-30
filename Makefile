CC                  = gcc
CFLAGS              = -Wall -Wextra -Werror
LIBMLX_DIR          = lib/libmlx
LIBMLX              = $(LIBMLX_DIR)/libmlx.a
LIBMLX_FLAGS        = -L$(LIBMLX_DIR) -lmlx -framework OpenGL -framework AppKit
SRC_DIR             = src
BUILD_DIR           = build
BIN_DIR             = $(BUILD_DIR)/bin
INC_DIR             = -Iinc -I$(LIBMLX_DIR)
RM                  = rm -rf

# DDA Program
DDA_PROGRAM         = dda
DDA_SRCS            = $(SRC_DIR)/DDA_algorithm.c
DDA_OBJS            = $(DDA_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Raycasting Program
RAYCASTING_PROGRAM  = raycasting
RAYCASTING_SRCS     = $(SRC_DIR)/deneme2.c
RAYCASTING_OBJS     = $(RAYCASTING_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Derleme ve bağlama işlemleri
all: $(LIBMLX) $(DDA_PROGRAM) $(RAYCASTING_PROGRAM)

# DDA Program Derleme ve Bağlama
$(DDA_PROGRAM): $(DDA_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INC_DIR) $(LIBMLX_FLAGS) $(DDA_OBJS) -o $(BIN_DIR)/$(DDA_PROGRAM)

# Raycasting Program Derleme ve Bağlama
$(RAYCASTING_PROGRAM): $(RAYCASTING_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INC_DIR) $(LIBMLX_FLAGS) $(RAYCASTING_OBJS) -o $(BIN_DIR)/$(RAYCASTING_PROGRAM)

# Nesne dosyalarının oluşturulması
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@

# MinilibX'in derlenmesi
$(LIBMLX):
	@make -s -C $(LIBMLX_DIR)

# Temizleme işlemi
clean:
	$(RM) $(DDA_OBJS) $(RAYCASTING_OBJS)

fclean: clean
	$(RM) $(BIN_DIR)/$(DDA_PROGRAM) $(BIN_DIR)/$(RAYCASTING_PROGRAM) $(LIBMLX)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re
