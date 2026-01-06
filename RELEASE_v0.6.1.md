# TelegramDigger v0.6.1 Release Notes

**Release Date**: January 6, 2026
**Developed by**: KawaiiPantsu | THUGS(red) Hacking Community

---

## 🎯 Targeted Analysis Enhancement

TelegramDigger v0.6.1 adds powerful targeted chat analysis capabilities to the comprehensive `--analyze` feature introduced in v0.6.0.

## ✨ New in This Release

### 🔍 Targeted Chat/Group Analysis

Analyze specific chats and groups beyond what's in the bot's update queue:

- **`--groupid <ID>`** - Analyze specific group by ID
- **`--chatid <ID>`** - Analyze specific chat by ID
- **Multiple IDs** - Specify multiple chat IDs in a single command
- **Deep Analysis** - Full chat details, member counts, and administrator lists

```bash
# Analyze a specific group
telegramdigger --analyze --groupid -1001234567890 --token "YOUR_TOKEN"

# Analyze multiple chats
telegramdigger --analyze --chatid 123456789 --chatid 987654321 --token "YOUR_TOKEN"

# Combine with environment variable
export TGDIGGER_TOKEN="YOUR_TOKEN"
telegramdigger --analyze --groupid -1001234567890 --chatid 123456789
```

### 📚 Enhanced Documentation

- **Examples Section** - Comprehensive usage examples in help screen
- **Better Guidance** - Clear examples for common use cases
- **Usage Patterns** - Demonstrates validation, analysis, and targeted analysis

### 🛡️ Validation & Error Handling

- **Chat ID Validation** - Ensures numeric input with helpful error messages
- **Progress Indicators** - Shows when additional chats are being analyzed
- **Clear Feedback** - User-friendly error messages with examples

## 🎯 Use Cases

### Security Auditing
```bash
# Audit a bot's access to specific sensitive groups
telegramdigger --analyze --groupid -1001234567890 --groupid -1009876543210
```

### Incident Response
```bash
# Quickly analyze suspicious groups where a bot is active
telegramdigger --analyze --chatid 123456789 --token "COMPROMISED_TOKEN"
```

### Comprehensive Testing
```bash
# Analyze all known chats associated with a bot
telegramdigger --analyze \
  --groupid -1001111111111 \
  --groupid -1002222222222 \
  --chatid 333333333 \
  --chatid 444444444
```

## 📋 Updated Feature List

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

## 🔧 Technical Improvements

- Enhanced CLI argument parsing for multiple chat IDs
- Improved chat enumeration logic to merge specified and discovered chats
- Progress feedback for additional chat analysis
- Input validation with exception handling

## 📦 Installation

### Upgrade from v0.6.0

```bash
# If installed from source
cd telegramdigger
git pull
make clean && make
sudo make install

# If using DEB package
wget https://github.com/kawaiipantsu/telegramdigger/releases/download/v0.6.1/telegramdigger_0.6.1_amd64.deb
sudo dpkg -i telegramdigger_0.6.1_amd64.deb
```

### Fresh Installation

```bash
git clone https://github.com/kawaiipantsu/telegramdigger.git
cd telegramdigger
make
sudo make install
```

**Prerequisites**: `build-essential`, `libcurl4-openssl-dev`

## 🚀 Quick Start Guide

```bash
# Set your bot token
export TGDIGGER_TOKEN="123456789:ABCdefGHIjklMNOpqrsTUVwxyz"

# Run comprehensive analysis
telegramdigger --analyze

# Analyze specific groups
telegramdigger --analyze --groupid -1001234567890

# Analyze multiple chats
telegramdigger --analyze --chatid 123456789 --chatid 987654321

# View the generated report
cat ~/.telegramdigger/reports/*.md
```

## 📖 What's New Since v0.6.0

### v0.6.1 Additions
- ✨ `--groupid` flag for targeted group analysis
- ✨ `--chatid` flag for targeted chat analysis
- ✨ Support for multiple chat IDs in single command
- ✨ Enhanced examples section in help screen
- 🛡️ Input validation for chat IDs
- 📊 Progress indicators for additional chat analysis

### v0.6.0 Major Features (Previous Release)
- 🔍 Comprehensive OSINT analysis (`--analyze`)
- 🛡️ Security weakness detection (4 categories)
- 📄 Professional markdown report generation
- 📊 8 new Telegram API methods
- 🔐 Privacy and security auditing
- 📈 Real-time progress indicators

## ⚖️ Legal & Ethics

**IMPORTANT**: This tool is intended for:
- ✅ Authorized security testing and penetration testing
- ✅ CTF (Capture The Flag) challenges
- ✅ Educational and research purposes
- ✅ Defensive security analysis

**You must have explicit permission to test any bot tokens and analyze any chats that you do not own or manage.**

Unauthorized access to computer systems and private chats is illegal. Use this tool responsibly and ethically.

## 🐛 Known Issues

None reported in this release.

## 📞 Support & Contributing

- **Issues**: https://github.com/kawaiipantsu/telegramdigger/issues
- **Email**: thugsred@protonmail.com
- **Community**: THUGS(red) Hacking Community

Contributions, bug reports, and feature requests are welcome!

## 🔮 Coming Soon

Potential features for future releases:
- User-based analysis and tracking
- Export formats (JSON, CSV, XML)
- Automated scheduling for periodic analysis
- Comparison reports between analysis runs
- Advanced filtering and search in reports

---

**Developed with ❤️ for the security research community**

*KawaiiPantsu | THUGS(red) Hacking Community*
