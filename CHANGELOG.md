# Changelog

All notable changes to TelegramDigger will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.7.0] - 2026-01-07

### Added

#### Token Annotation System
- **`--note` option** for `--validate` command
  - Add custom notes to tokens in the tokens-seen file
  - Notes appended as third field in CSV format: `token#timestamp#note`
  - Useful for tracking token sources and context
  - Example: `--validate --token TOKEN --note "Found on production server"`

#### Message Sending System (`--send-message`)
- **Complete message sending functionality**
  - `--send-message <TEXT>` - Send text messages to any chat
  - `--chatid <ID>` - Specify target chat ID (private, group, or channel)
  - Support for all chat types (private chats, groups, supergroups, channels)
  - Message ID returned on successful send

- **Advanced Formatting Support**
  - `--parse-mode <MODE>` - Format messages with Markdown, MarkdownV2, or HTML
  - **Markdown**: `*bold*`, `_italic_`, `` `code` ``, `[link](URL)`
  - **MarkdownV2**: Enhanced Markdown with `__underline__`, `~strikethrough~`, `||spoiler||`
  - **HTML**: `<b>`, `<i>`, `<u>`, `<code>`, `<pre>`, `<a>` tags
  - Full URL encoding for special characters
  - Comprehensive formatting guide in FORMATTING.md

- **Message Delivery Options**
  - `--silent` - Send messages without notifications (no sound/vibration)
    - Perfect for automated monitoring
    - Useful for off-hours alerts
    - Low-priority status updates
  - `--nopreview` - Disable link preview generation
    - Cleaner messages with multiple URLs
    - Security best practice for suspicious links
    - Faster message delivery

- **Examples**:
  ```bash
  # Send plain text message
  telegramdigger --send-message "Hello World" --chatid 123456789

  # Send formatted HTML message
  telegramdigger --send-message "<b>Alert:</b> Token found" --chatid 123 --parse-mode HTML

  # Send silent message without link preview
  telegramdigger --send-message "Check https://example.com" --chatid 123 --silent --nopreview
  ```

#### Documentation
- **FORMATTING.md** - Comprehensive formatting guide
  - Complete syntax reference for all three formatting modes
  - Real-world examples for security alerts, monitoring, and automation
  - Best practices for silent messages and link previews
  - Tips for choosing the right formatting mode
  - Common use cases with working examples

### Improved
- Help screen updated with message sending options
- Extended examples section with formatting demonstrations
- Enhanced output display showing parse mode, silent status, and preview settings
- Better error messages for missing parameters

### Technical
- Extended `TelegramApi::sendMessage()` with formatting and delivery options
- Added URL encoding for message text
- Implemented boolean flags for notification and preview control
- New argument parsing functions: `getParseMode()`, `hasSilentFlag()`, `hasNoPreviewFlag()`
- Updated `Config::saveTokenSeen()` to support optional notes

## [0.6.1] - 2026-01-06

### Added
- **Targeted Chat Analysis** for `--analyze` command
  - `--groupid <ID>` option to analyze specific groups
  - `--chatid <ID>` option to analyze specific chats
  - Support for multiple chat IDs in a single command
  - Enhanced analysis includes user-specified chats beyond update queue
  - Input validation with helpful error messages for invalid chat IDs
  - Progress indicator showing when additional chats are being analyzed

### Improved
- Help screen now includes comprehensive usage examples
- Examples section demonstrates various analysis scenarios
- Better documentation for targeted analysis workflows

## [0.6.0] - 2026-01-06

### Added

#### Comprehensive Bot Analysis (`--analyze`)
- **OSINT Analysis System**
  - 8-phase analysis process with progress indicators
  - Comprehensive data collection from multiple Telegram API endpoints
  - Automatic report generation in markdown format
  - Secure report storage in `~/.telegramdigger/reports/<token>.md` (0600 permissions)

- **Security Weakness Detection**
  - **Webhook Security Analysis**
    - Detection of HTTP webhooks (insecure protocol)
    - Identification of high pending update counts (>50)
    - Webhook IP address exposure tracking
    - Warning for missing webhook configuration
  - **Permission Issues Analysis**
    - Privacy mode disabled detection (can read all group messages)
    - Excessive administrator rights identification
    - Bot activity in too many groups (>10)
    - Configuration inconsistency detection
  - **Data Exposure Analysis**
    - Unprocessed updates containing user data (>10 pending)
    - Sensitive information in bot descriptions
    - Multiple user identities in update queue
    - Username and identity exposure tracking
  - **Configuration Gaps Analysis**
    - Missing bot commands detection
    - Missing bot descriptions
    - Incomplete bot setup identification
    - Contradictory settings detection

- **Data Collection**
  - Bot information (name, username, capabilities)
  - Administrator rights (groups and channels)
  - Webhook configuration and status
  - Bot commands list
  - Bot descriptions (full and short)
  - Display name
  - Pending updates (up to 100)
  - Chat enumeration from updates
  - Deep chat analysis (info, member counts, administrators)
  - Unique user tracking

- **New Telegram API Methods**
  - `getMyCommands()` - Fetch configured bot commands
  - `getMyDescription()` - Get bot description
  - `getMyName()` - Get bot display name
  - `getMyShortDescription()` - Get short description
  - `getUpdates()` - Fetch pending updates with customizable limit
  - `getChat()` - Get detailed chat information
  - `getChatAdministrators()` - Get chat administrator list
  - `getChatMemberCount()` - Get member count for chats

- **Professional Markdown Reports**
  - Executive summary with severity counts
  - Detailed security findings with recommendations
  - Bot configuration analysis
  - Webhook status and configuration
  - Updates breakdown by type
  - Chat analysis with administrator lists
  - Timestamped report generation
  - Token masking for security

- **Security Finding Categories**
  - CRITICAL - Immediate security risks
  - HIGH - Significant security concerns
  - MEDIUM - Notable security issues
  - LOW - Minor security improvements
  - INFO - Informational findings

- **Terminal Output**
  - Real-time progress indicators (8 phases)
  - Color-coded security findings display
  - Statistics summary (pending updates, unique users, active chats)
  - Success/error messaging
  - UTF-8 icon support for enhanced visuals

### Technical Improvements

#### Data Structures
- `BotCommand` - Bot command information storage
- `Update` - Update data with multi-type support
- `ChatInfo` - Comprehensive chat details
- `ChatAdmin` - Administrator information
- `SecurityFinding` - Security issue tracking
- `BotAnalysis` - Complete analysis container

#### JSON Parsing Enhancement
- Array parsing for commands and administrators
- Nested object extraction for updates
- Multi-level JSON navigation
- Support for complex update types (message, edited_message, channel_post, callback_query)

#### Analysis Engine
- Modular security weakness detection
- Configurable severity thresholds
- Extensible finding system
- Context-aware analysis rules
- Privacy-focused checks

### Documentation
- Updated help screen with `--analyze` option
- Comprehensive examples in help display
- Token masking in reports for security
- Clear error messages for API failures

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
