# TelegramDigger v0.6.1 Release Notes

**Release Date**: January 6, 2026
**Developed by**: KawaiiPantsu | THUGS(red) Hacking Community

---

## 🎯 Targeted Analysis Enhancement

TelegramDigger v0.6.1 enhances the comprehensive analysis feature with targeted chat and group analysis capabilities, allowing security researchers to dig deeper into specific chats beyond the bot's update queue.

> **📌 For v0.5.0 release notes, see [RELEASE_v0.5.0.md](RELEASE_v0.5.0.md)**
> **📌 For v0.6.0 release notes, see [RELEASE_v0.6.0.md](RELEASE_v0.6.0.md)**

## ✨ Key Features

### 🎯 Targeted Chat Analysis (NEW in v0.6.1!)
Deep dive into specific chats and groups:

- **Targeted Group Analysis** - Analyze specific groups by ID
- **Targeted Chat Analysis** - Analyze specific chats by ID
- **Multiple IDs** - Specify multiple chat IDs in a single command
- **Deep Analysis** - Full chat details, member counts, administrator lists

```bash
telegramdigger --analyze --groupid -1001234567890 --token "YOUR_TOKEN"
telegramdigger --analyze --chatid 123456789 --chatid 987654321
```

### 🔍 Comprehensive Bot Analysis (NEW in v0.6.0!)
LinPEAS-style OSINT analysis for Telegram bots:

- **8-Phase Analysis** - Progressive data collection with real-time indicators
- **Security Weakness Detection** - 4 categories (Webhook, Permissions, Data Exposure, Configuration)
- **Professional Reports** - Markdown reports saved to `~/.telegramdigger/reports/`
- **Deep Chat Enumeration** - Automatic discovery and analysis of bot's chats

```bash
telegramdigger --analyze --token "YOUR_TOKEN"
```

### 🔗 Webhook Management
Complete webhook configuration management for Telegram bots:

- **Get Webhook Status** - View current webhook configuration, pending updates, and error logs
- **Set Webhook URL** - Configure HTTPS endpoint for receiving bot updates
- **Delete Webhook** - Remove webhook and switch back to long polling

```bash
telegramdigger --webhook-get --token "YOUR_TOKEN"
telegramdigger --webhook-set https://example.com/webhook --token "YOUR_TOKEN"
telegramdigger --webhook-delete --token "YOUR_TOKEN"
```

### 👑 Bot Administrator Rights (NEW!)
Analyze default administrator permissions for your bots:

- Separate rights for Groups/Supergroups and Channels
- 12 different permission flags
- Color-coded display for easy reading

```bash
telegramdigger --read-botrights --token "YOUR_TOKEN"
```

### 📊 Bulk Token Validation (NEW!)
Validate multiple tokens efficiently:

- Process tokens from files (one per line)
- CSV format support for token history
- Progress tracking and summary statistics
- Automatic comment filtering

```bash
telegramdigger --bulk-validate tokens.txt
```

### 🔍 Token Validation & Analysis
Core security testing capabilities:

- Token format validation (bot_id:token_hash)
- API validation via Telegram Bot API
- Bot information extraction (ID, name, username, capabilities)
- Automatic token tracking and storage

```bash
telegramdigger --validate --token "YOUR_TOKEN"
```

## 💾 Token Tracking System

Automatic storage and tracking of validated tokens:

- **Valid Tokens**: Stored in `~/.telegramdigger/valid-tokens/` with full bot info
- **Token History**: CSV log in `~/.telegramdigger/tokens-seen` with timestamps
- **Secure Permissions**: 0600 (owner read/write only) for all sensitive data

## 🎨 User Experience

- **Styled Terminal Output**: ANSI 256-color support with UTF-8 icons
- **Professional ASCII Art**: Custom shovel/dirt theme header
- **Clear Error Messages**: Helpful guidance for troubleshooting
- **Multiple Input Methods**: CLI arguments, environment variables, or config file

## 🔐 Security Hardening

Built with security in mind:

- Stack buffer overflow protection
- Position Independent Executable (PIE)
- Read-only relocations (RELRO)
- Fortified source compilation
- HTTPS-only API communication with SSL verification

## 📦 Installation

### Debian/Ubuntu (DEB Package)

```bash
wget https://github.com/kawaiipantsu/telegramdigger/releases/download/v0.5.0/telegramdigger_0.5.0_amd64.deb
sudo dpkg -i telegramdigger_0.5.0_amd64.deb
```

### Build from Source

```bash
git clone https://github.com/kawaiipantsu/telegramdigger.git
cd telegramdigger
make
sudo make install
```

**Prerequisites**: `build-essential`, `libcurl4-openssl-dev`

## 🚀 Quick Start

```bash
# Set your bot token
export TGDIGGER_TOKEN="123456789:ABCdefGHIjklMNOpqrsTUVwxyz"

# Validate the token
telegramdigger --validate

# Check administrator rights
telegramdigger --read-botrights

# Get webhook status
telegramdigger --webhook-get

# Show help for all options
telegramdigger --help
```

## 📋 Complete Feature List

| Feature | Command | Description |
|---------|---------|-------------|
| **Comprehensive Analysis** | `--analyze` | Full OSINT analysis with security audit |
| **Targeted Group Analysis** | `--groupid <ID>` | Analyze specific group (with --analyze) |
| **Targeted Chat Analysis** | `--chatid <ID>` | Analyze specific chat (with --analyze) |
| Token Validation | `--validate` | Validate bot token via API |
| Bulk Validation | `--bulk-validate [FILE]` | Validate multiple tokens |
| Bot Rights | `--read-botrights` | View administrator permissions |
| Get Webhook | `--webhook-get` | View webhook configuration |
| Set Webhook | `--webhook-set <URL>` | Configure webhook URL |
| Delete Webhook | `--webhook-delete` | Remove webhook |
| Help | `--help` | Show help message with examples |
| Version | `--version` | Show version info |
| About | `--about` | Show about information |

## 🔧 Technical Details

- **Language**: C++17
- **Build System**: GNU Make
- **Dependencies**: libcurl4
- **Package Size**: ~50 KB
- **Binary Size**: ~168 KB
- **Architecture**: amd64 (x86_64)

## 📖 Documentation

Full documentation available in the repository:
- [README.md](README.md) - Complete usage guide
- [CHANGELOG.md](CHANGELOG.md) - Detailed change history

## ⚖️ Legal & Ethics

**IMPORTANT**: This tool is intended for:
- ✅ Authorized security testing and penetration testing
- ✅ CTF (Capture The Flag) challenges
- ✅ Educational and research purposes
- ✅ Defensive security analysis

**You must have explicit permission to test any bot tokens that you do not own.**

Unauthorized access to computer systems is illegal. Use this tool responsibly and ethically.

## 🐛 Known Issues

None reported in this release.

## 📞 Support & Contributing

- **Issues**: https://github.com/kawaiipantsu/telegramdigger/issues
- **Email**: thugsred@protonmail.com
- **Community**: THUGS(red) Hacking Community

Contributions, bug reports, and feature requests are welcome!

## 🙏 Acknowledgments

Special thanks to the THUGS(red) Hacking Community for their support and feedback during development.

---

**Developed with ❤️ for the security research community**

*KawaiiPantsu | THUGS(red) Hacking Community*
