# Changelog

All notable changes to TelegramDigger will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.5.0] - 2026-01-05

### Added

#### Webhook Management
- **Get Webhook Information** (`--webhook-get`, `--webhook-info`)
  - Retrieve current webhook configuration from Telegram API
  - Display webhook URL, SSL certificate status, pending updates count
  - Show IP address and max connections settings
  - Display last error information with timestamp if webhook failed
  - User-friendly message when no webhook is configured

- **Set Webhook** (`--webhook-set <URL>`)
  - Configure webhook URL for receiving bot updates via HTTPS POST
  - Support for custom max connections parameter (default: 40)
  - Validation of required URL parameter
  - Helpful reminders about HTTPS and SSL certificate requirements
  - Clear error messages for failed operations

- **Delete Webhook** (`--webhook-delete`)
  - Remove webhook configuration to switch back to long polling
  - Confirmation message explaining transition to getUpdates method
  - Clean removal of webhook settings from Telegram servers

#### Bot Administrator Rights
- **Read Bot Rights** (`--read-botrights`)
  - Retrieve default administrator rights for groups/supergroups and channels
  - Display all 12 permission flags with color-coded Yes/No indicators
  - Separate sections for Groups/Supergroups and Channels
  - Channel-specific permissions: Post Messages and Edit Messages

#### Token Management
- **Bulk Token Validation** (`--bulk-validate [FILE]`)
  - Validate multiple tokens from a file (one per line)
  - Support for CSV format (token#date) used by tokens-seen
  - Comment support (lines starting with #)
  - Progress indicator showing current token number (e.g., [2/5])
  - Summary statistics: total tokens, valid count, invalid count
  - Truncated token display for security (e.g., 123456789:ABCde...)
  - Display bot name and username for valid tokens

- **Token Tracking System**
  - Automatic storage of validated tokens in `~/.telegramdigger/valid-tokens/`
  - CSV log of all seen tokens in `~/.telegramdigger/tokens-seen`
  - Timestamp tracking for each token validation
  - Detailed bot information saved for valid tokens
  - Secure file permissions (0600) for sensitive data

#### Core Features
- **Token Validation** (`--validate`)
  - Format validation for Telegram bot token structure
  - API validation via getMe endpoint
  - Detailed bot information extraction (ID, name, username, capabilities)
  - Bot capabilities display (join groups, read messages, inline queries)
  - Automatic token storage upon successful validation

- **Bot Information Display**
  - Structured display of bot ID, name, and username
  - Bot type verification (is_bot flag)
  - Capability flags: Can Join Groups, Can Read All Messages, Supports Inline Queries
  - Color-coded Yes/No indicators for easy reading

### Technical Improvements

#### HTTP Client
- libcurl-based HTTP/HTTPS client implementation
- SSL certificate verification enabled by default
- Configurable timeout settings (default: 30 seconds)
- Error handling and reporting for network failures
- PIMPL pattern for clean API surface

#### Terminal Enhancements
- ANSI 256-color support with automatic capability detection
- UTF-8 icon support (Nerd Fonts/Geek Fonts compatible)
- Consistent styled output across all features
- Color-coded success (green), error (red), warning (yellow), info (cyan) messages
- Professional ASCII art header with shovel/dirt theme

#### Configuration System
- Singleton pattern for global configuration access
- Multiple token input methods:
  1. Command-line argument: `--token <TOKEN>`
  2. Environment variable: `TGDIGGER_TOKEN`
  3. Config file: `~/.telegramdigger/settings.conf`
- Automatic config directory creation with secure permissions (0700)
- Key-value format for settings file

#### Security Features
- Stack buffer overflow protection (`-fstack-protector-strong`)
- Position Independent Executable (PIE)
- Read-only relocations (RELRO)
- Format string vulnerability detection
- Fortified source compilation (`-D_FORTIFY_SOURCE=2`)
- Secure file permissions for all sensitive data
- HTTPS-only communication with Telegram API

#### Build System
- Modern C++17 implementation
- GNU Make build system with automatic dependency tracking
- DEB package generation for Debian/Ubuntu distributions
- Clean separation of source, headers, and build artifacts
- Optimized compilation with `-O2` flag
- Multiple build targets: build, clean, install, deb, help

### Documentation

- Comprehensive README.md with:
  - Installation instructions for Debian/Ubuntu
  - Usage examples for all features
  - Token format specification
  - Security considerations
  - Configuration file structure
  - Command-line options reference
  - Output examples

- Inline code documentation:
  - Function-level documentation comments
  - Clear variable naming conventions
  - Structured code organization

- Help system:
  - Detailed `--help` screen with ASCII art
  - `--version` command showing terminal capabilities
  - `--about` command with author and purpose information

### Package Information

- **Version**: 0.5.0
- **Package Size**: ~50 KB (DEB)
- **Binary Size**: ~168 KB
- **Architecture**: amd64
- **Dependencies**: libcurl4

### Credits

- **Developer**: KawaiiPantsu
- **Organization**: THUGS(red) Hacking Community
- **Contact**: thugsred@protonmail.com
- **Repository**: https://github.com/kawaiipantsu/telegramdigger

### Legal

This tool is intended for:
- Authorized security testing and penetration testing
- CTF (Capture The Flag) challenges
- Educational and research purposes
- Defensive security analysis

**You must have explicit permission to test any bot tokens that you do not own.**

---

## [0.1.0] - Initial Development

### Added
- Project structure and build system
- Basic CLI framework
- Terminal styling utilities
- Configuration management skeleton
