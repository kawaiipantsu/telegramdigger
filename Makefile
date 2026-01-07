# TelegramDigger Makefile
# C++ Security Testing Tool for Telegram Bot Tokens

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 \
            -fstack-protector-strong -D_FORTIFY_SOURCE=2 \
            -fPIE -Wformat -Wformat-security
LDFLAGS := -pie -Wl,-z,relro,-z,now
LIBS := -lcurl
INCLUDES := -Iinclude

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := bin
DEBIAN_DIR := debian

# Target
TARGET := telegramdigger
BINARY := $(BIN_DIR)/$(TARGET)

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SOURCES))

# Installation paths
PREFIX ?= /usr/local
BINDIR := $(PREFIX)/bin
MANDIR := $(PREFIX)/share/man/man1

# Version information
VERSION := 0.7.0
PACKAGE_NAME := telegramdigger
ARCH := $(shell dpkg --print-architecture)
DEB_PACKAGE := $(PACKAGE_NAME)_$(VERSION)_$(ARCH).deb

# Colors for output
COLOR_RESET := \033[0m
COLOR_GREEN := \033[32m
COLOR_YELLOW := \033[33m
COLOR_CYAN := \033[36m
COLOR_BOLD := \033[1m

# Default target
.PHONY: all
all: $(BINARY)

# Create directories
$(BUILD_DIR) $(BIN_DIR):
	@mkdir -p $@

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "$(COLOR_CYAN)$(COLOR_BOLD)[CC]$(COLOR_RESET) $(COLOR_GREEN)$<$(COLOR_RESET)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Link binary
$(BINARY): $(OBJECTS) | $(BIN_DIR)
	@echo "$(COLOR_CYAN)$(COLOR_BOLD)[LD]$(COLOR_RESET) $(COLOR_GREEN)$@$(COLOR_RESET)"
	@$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)✓ Build complete: $(BINARY)$(COLOR_RESET)"

# Include dependency files
-include $(DEPENDS)

# Install target
.PHONY: install
install: $(BINARY)
	@echo "$(COLOR_CYAN)$(COLOR_BOLD)[INSTALL]$(COLOR_RESET) Installing $(TARGET)..."
	@install -d $(DESTDIR)$(BINDIR)
	@install -m 0755 $(BINARY) $(DESTDIR)$(BINDIR)/$(TARGET)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)✓ Installed to $(DESTDIR)$(BINDIR)/$(TARGET)$(COLOR_RESET)"

# Uninstall target
.PHONY: uninstall
uninstall:
	@echo "$(COLOR_YELLOW)$(COLOR_BOLD)[UNINSTALL]$(COLOR_RESET) Removing $(TARGET)..."
	@rm -f $(DESTDIR)$(BINDIR)/$(TARGET)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)✓ Uninstalled$(COLOR_RESET)"

# Clean build artifacts
.PHONY: clean
clean:
	@echo "$(COLOR_YELLOW)$(COLOR_BOLD)[CLEAN]$(COLOR_RESET) Removing build artifacts..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)✓ Clean complete$(COLOR_RESET)"

# Clean everything including DEB packages
.PHONY: distclean
distclean: clean
	@echo "$(COLOR_YELLOW)$(COLOR_BOLD)[DISTCLEAN]$(COLOR_RESET) Removing packages..."
	@rm -f *.deb
	@rm -rf $(DEBIAN_DIR)/$(PACKAGE_NAME)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)✓ Distclean complete$(COLOR_RESET)"

# Run the application
.PHONY: run
run: $(BINARY)
	@$(BINARY)

# Build DEB package
.PHONY: deb
deb: $(BINARY)
	@echo "$(COLOR_CYAN)$(COLOR_BOLD)[DEB]$(COLOR_RESET) Building Debian package..."
	@$(MAKE) deb-prepare
	@dpkg-deb --build $(DEBIAN_DIR)/$(PACKAGE_NAME) $(DEB_PACKAGE)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)✓ Package created: $(DEB_PACKAGE)$(COLOR_RESET)"
	@$(MAKE) deb-clean

# Prepare DEB package structure
.PHONY: deb-prepare
deb-prepare:
	@echo "$(COLOR_CYAN)[DEB]$(COLOR_RESET) Preparing package structure..."
	@rm -rf $(DEBIAN_DIR)/$(PACKAGE_NAME)
	@mkdir -p $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN
	@mkdir -p $(DEBIAN_DIR)/$(PACKAGE_NAME)/usr/bin
	@mkdir -p $(DEBIAN_DIR)/$(PACKAGE_NAME)/usr/share/doc/$(PACKAGE_NAME)
	@cp $(BINARY) $(DEBIAN_DIR)/$(PACKAGE_NAME)/usr/bin/
	@strip $(DEBIAN_DIR)/$(PACKAGE_NAME)/usr/bin/$(TARGET)
	@chmod 755 $(DEBIAN_DIR)/$(PACKAGE_NAME)/usr/bin/$(TARGET)
	@$(MAKE) deb-control

# Create DEB control file
.PHONY: deb-control
deb-control:
	@echo "$(COLOR_CYAN)[DEB]$(COLOR_RESET) Creating control file..."
	@echo "Package: $(PACKAGE_NAME)" > $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN/control
	@echo "Version: $(VERSION)" >> $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN/control
	@echo "Section: utils" >> $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN/control
	@echo "Priority: optional" >> $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN/control
	@echo "Architecture: $(ARCH)" >> $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN/control
	@echo "Depends: libcurl4" >> $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN/control
	@echo "Maintainer: THUGS(red) Hacking Community" >> $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN/control
	@echo "Description: Security testing tool for Telegram bot tokens" >> $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN/control
	@echo " TelegramDigger is a tool for exploring and testing Telegram bot tokens" >> $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN/control
	@echo " discovered during security assessments. It supports read/write operations" >> $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN/control
	@echo " to the Bot API and analysis of bot token security." >> $(DEBIAN_DIR)/$(PACKAGE_NAME)/DEBIAN/control
	@if [ -f README.md ]; then \
		cp README.md $(DEBIAN_DIR)/$(PACKAGE_NAME)/usr/share/doc/$(PACKAGE_NAME)/; \
	fi

# Clean DEB build artifacts
.PHONY: deb-clean
deb-clean:
	@rm -rf $(DEBIAN_DIR)/$(PACKAGE_NAME)

# Help target
.PHONY: help
help:
	@echo "$(COLOR_CYAN)$(COLOR_BOLD)TelegramDigger Build System$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_YELLOW)Available targets:$(COLOR_RESET)"
	@echo "  $(COLOR_GREEN)all$(COLOR_RESET)         - Build the application (default)"
	@echo "  $(COLOR_GREEN)clean$(COLOR_RESET)       - Remove build artifacts"
	@echo "  $(COLOR_GREEN)distclean$(COLOR_RESET)   - Remove build artifacts and packages"
	@echo "  $(COLOR_GREEN)install$(COLOR_RESET)     - Install to $(PREFIX)"
	@echo "  $(COLOR_GREEN)uninstall$(COLOR_RESET)   - Remove installed files"
	@echo "  $(COLOR_GREEN)run$(COLOR_RESET)         - Build and run the application"
	@echo "  $(COLOR_GREEN)deb$(COLOR_RESET)         - Build Debian package"
	@echo "  $(COLOR_GREEN)help$(COLOR_RESET)        - Show this help message"
	@echo ""
	@echo "$(COLOR_YELLOW)Variables:$(COLOR_RESET)"
	@echo "  PREFIX      - Installation prefix (default: /usr/local)"
	@echo "  DESTDIR     - Destination directory for staged installs"
	@echo ""
	@echo "$(COLOR_YELLOW)Examples:$(COLOR_RESET)"
	@echo "  make                    - Build the application"
	@echo "  make clean              - Clean build artifacts"
	@echo "  make install            - Install to /usr/local/bin"
	@echo "  make PREFIX=/usr install - Install to /usr/bin"
	@echo "  make deb                - Create DEB package"
