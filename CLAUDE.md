# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

TeleGram Digger is a security testing tool for working with Telegram bot tokens. It provides:
- API exploration capabilities for discovered bot tokens
- Read/write operations to the Telegram Bot API
- Features for analyzing and testing bot token security

**Important**: This tool is intended for authorized security testing, CTF challenges, and educational contexts only. It should only be used with bot tokens you own or have explicit permission to test.

## Language and Platform

- **Language**: C++17
- **Platform**: Linux (Debian-based distributions)
- **Compiler**: g++ with security hardening flags
- **Build System**: Make

## Building the Project

### Prerequisites
```bash
sudo apt-get install build-essential dpkg-dev libcurl4-openssl-dev
```

**Dependencies:**
- build-essential (g++, make, etc.)
- dpkg-dev (for DEB packaging)
- libcurl4-openssl-dev (for HTTP API requests)

### Build Commands
```bash
# Build the application
make

# Build and run
make run

# Clean build artifacts
make clean

# Build DEB package
make deb

# Install system-wide (requires sudo)
sudo make install

# Install to custom prefix
make PREFIX=/opt/telegramdigger install

# Uninstall
sudo make uninstall
```

### Build Targets
- `make` or `make all` - Compile the application
- `make clean` - Remove build artifacts
- `make distclean` - Remove build artifacts and packages
- `make install` - Install to system (default: /usr/local/bin)
- `make uninstall` - Remove installed files
- `make run` - Build and run with default help screen
- `make deb` - Create Debian package
- `make help` - Show all available targets

## Project Structure

```
telegramdigger/
├── src/            # Source files (.cpp)
│   ├── main.cpp           # Entry point and CLI argument parsing
│   ├── terminal.cpp       # Terminal styling (ANSI colors, UTF-8 icons)
│   ├── config.cpp         # Configuration management
│   ├── http_client.cpp    # HTTP client using libcurl
│   └── telegram_api.cpp   # Telegram Bot API client
├── include/        # Header files (.h)
│   ├── terminal.h         # Terminal utilities API
│   ├── config.h           # Config manager API
│   ├── version.h          # Version constants
│   ├── http_client.h      # HTTP client interface
│   └── telegram_api.h     # Telegram API interface
├── build/          # Compiled object files (generated)
├── bin/            # Binary output (generated)
├── debian/         # DEB packaging temp directory
└── Makefile        # Build system
```

## Architecture

### Core Components

**Terminal Utilities** (`terminal.h/cpp`)
- ANSI 256-color support with fallback
- UTF-8 Geek Font icon rendering (if terminal supports it)
- Helper methods for styled output (error, success, warning, info)
- Terminal capability detection (UTF-8, 256-color)

**Configuration Manager** (`config.h/cpp`)
- Singleton pattern for global config access
- Reads/writes `$HOME/.telegramdigger/settings.conf`
- Simple key=value format with comment support
- Auto-creates config directory with secure permissions (0700)
- Config file secured with 0600 permissions

**HTTP Client** (`http_client.h/cpp`)
- PIMPL idiom for libcurl abstraction
- Support for GET and POST requests
- Configurable timeouts and SSL verification
- Custom header support
- Thread-safe implementation

**Telegram API Client** (`telegram_api.h/cpp`)
- Bot token validation via getMe endpoint
- Token format validation (basic and API-based)
- Simple JSON parser for API responses (no external dependencies)
- Structured bot information retrieval
- Error handling and reporting

**Main Application** (`main.cpp`)
- Command-line argument parsing with --token and --validate
- Token retrieval from multiple sources (CLI, env, config file)
- Help screen with ASCII art and colored output
- Version and about information displays
- Bot token validation workflow with detailed output

### Security Considerations

The build system includes security hardening:
- `-fstack-protector-strong` - Stack buffer overflow protection
- `-D_FORTIFY_SOURCE=2` - Compile-time buffer overflow detection
- `-fPIE` + `-pie` - Position Independent Executable
- `-Wl,-z,relro,-z,now` - Read-only relocations and immediate binding
- `-Wformat -Wformat-security` - Format string vulnerability detection

Configuration files are created with restricted permissions:
- Config directory: 0700 (owner only)
- Config file: 0600 (owner read/write only)

### Terminal Styling

The application automatically detects terminal capabilities:
- Checks `$LANG` for UTF-8 support
- Checks `$TERM` for 256-color support
- Gracefully degrades if features unavailable
- Uses Geek Font icons (Nerd Fonts) when UTF-8 is available

### Configuration Storage

User configuration is stored in `$HOME/.telegramdigger/settings.conf` with format:
```
# Comments start with # or ;
key=value
another_key=another value
```

Access via the singleton:
```cpp
Config& config = Config::getInstance();
config.load();
std::string value = config.get("key", "default");
config.set("key", "new_value");
config.save();
```

## Using Token Validation

The application supports bot token validation through the Telegram Bot API:

```bash
# Using command-line argument
./bin/telegramdigger --validate --token "YOUR_BOT_TOKEN"

# Using environment variable
export TGDIGGER_TOKEN="YOUR_BOT_TOKEN"
./bin/telegramdigger --validate

# Using config file
echo "bot_token=YOUR_BOT_TOKEN" >> ~/.telegramdigger/settings.conf
./bin/telegramdigger --validate
```

**Token Format:**
Telegram bot tokens follow the format: `<bot_id>:<token_hash>`
- Example: `123456789:ABCdefGHIjklMNOpqrsTUVwxyz-1234567890`

**Validation Process:**
1. Token format is validated (bot_id must be numeric, token_hash alphanumeric)
2. HTTPS request to `https://api.telegram.org/bot<token>/getMe`
3. Response is parsed to check if `"ok": true` is present
4. Bot information is displayed if valid (ID, name, username, capabilities)
5. Valid tokens are automatically saved for tracking

**Token Storage:**
When a token is successfully validated:
- Bot information saved to: `~/.telegramdigger/valid-tokens/<token>` (permissions: 0600)
- Token logged to: `~/.telegramdigger/tokens-seen` in CSV format: `<token>#<date>`
- Application tracks if tokens were previously seen

**Token Retrieval Priority:**
1. Command-line `--token` argument (highest priority)
2. `TGDIGGER_TOKEN` environment variable
3. `bot_token` in `~/.telegramdigger/settings.conf`

## Development Workflow

1. Make changes to source files in `src/` or headers in `include/`
2. Run `make` to compile
3. Run `make run` or `./bin/telegramdigger --help` to test
4. Run `make clean` before committing to remove build artifacts

## Adding New Features

When adding new functionality:
1. Create header in `include/` if defining a new class/module
2. Create implementation in `src/`
3. The Makefile automatically picks up new `.cpp` files in `src/`
4. Update version in `include/version.h` for releases
5. Follow the existing patterns for terminal output (use Terminal class methods)

## Packaging

To create a DEB package for distribution:
```bash
make deb
```

This creates `telegramdigger_0.1.0_<arch>.deb` which can be installed via:
```bash
sudo dpkg -i telegramdigger_0.1.0_<arch>.deb
```

The package installs to `/usr/bin/telegramdigger`
