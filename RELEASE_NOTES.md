# TelegramDigger v0.5.0 Release Notes

**Release Date**: January 5, 2026
**Developed by**: KawaiiPantsu | THUGS(red) Hacking Community

---

## 🎉 Major Release: Full-Featured Bot Token Analysis Tool

TelegramDigger v0.5.0 is the first major release, providing a comprehensive suite of tools for analyzing and testing Telegram bot tokens discovered during authorized security assessments.

## ✨ Key Features

### 🔗 Webhook Management (NEW!)
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
| Token Validation | `--validate` | Validate bot token via API |
| Bulk Validation | `--bulk-validate [FILE]` | Validate multiple tokens |
| Bot Rights | `--read-botrights` | View administrator permissions |
| Get Webhook | `--webhook-get` | View webhook configuration |
| Set Webhook | `--webhook-set <URL>` | Configure webhook URL |
| Delete Webhook | `--webhook-delete` | Remove webhook |
| Help | `--help` | Show help message |
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
