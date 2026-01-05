# Release v0.5.0 - Full-Featured Bot Token Analysis Tool

**First Major Release** 🎉

TelegramDigger v0.5.0 provides a comprehensive suite of tools for analyzing and testing Telegram bot tokens discovered during authorized security assessments.

## 🚀 What's New

### Webhook Management
Complete control over Telegram bot webhooks:
- `--webhook-get` / `--webhook-info` - View current webhook configuration and status
- `--webhook-set <URL>` - Configure HTTPS webhook endpoint for receiving updates
- `--webhook-delete` - Remove webhook configuration

### Bot Administrator Rights
Analyze default admin permissions:
- `--read-botrights` - Display all 12 permission flags for groups and channels
- Separate sections for Groups/Supergroups and Channels
- Color-coded Yes/No indicators

### Bulk Token Validation
Process multiple tokens efficiently:
- `--bulk-validate [FILE]` - Validate tokens from file (one per line)
- CSV format support with automatic timestamp tracking
- Progress indicators and summary statistics
- Displays bot names for valid tokens

### Token Validation & Tracking
Core security testing features:
- `--validate` - Validate token format and API status
- Automatic storage of validated tokens with metadata
- Secure CSV log in `~/.telegramdigger/tokens-seen`
- Bot information extraction (ID, name, username, capabilities)

## 📦 Installation

### Debian/Ubuntu
```bash
# Download the package
wget https://github.com/kawaiipantsu/telegramdigger/releases/download/v0.5.0/telegramdigger_0.5.0_amd64.deb

# Install
sudo dpkg -i telegramdigger_0.5.0_amd64.deb
```

### From Source
```bash
git clone https://github.com/kawaiipantsu/telegramdigger.git
cd telegramdigger
make
sudo make install
```

**Requirements**: `build-essential`, `libcurl4-openssl-dev`

## 🔧 Quick Start

```bash
# Set token via environment
export TGDIGGER_TOKEN="123456789:ABCdefGHI..."

# Validate the token
telegramdigger --validate

# Check admin rights
telegramdigger --read-botrights

# View webhook status
telegramdigger --webhook-get
```

## 📋 All Features

| Feature | Command |
|---------|---------|
| Token Validation | `--validate` |
| Bulk Validation | `--bulk-validate [FILE]` |
| Bot Rights | `--read-botrights` |
| Get Webhook | `--webhook-get` |
| Set Webhook | `--webhook-set <URL>` |
| Delete Webhook | `--webhook-delete` |

## 🔐 Security Features

- Stack protection and PIE enabled
- HTTPS-only communication with SSL verification
- Secure file permissions (0600/0700)
- No external dependencies except libcurl

## 🎨 User Experience

- ANSI 256-color terminal output
- UTF-8 icon support (Nerd Fonts)
- Professional ASCII art header
- Clear error messages and guidance
- Multiple token input methods (CLI, env, config)

## 📊 Technical Details

- **Language**: C++17
- **Package Size**: ~50 KB
- **Binary Size**: ~168 KB
- **Architecture**: amd64

## 📖 Documentation

- [README.md](README.md) - Complete usage guide
- [CHANGELOG.md](CHANGELOG.md) - Detailed change history

## ⚖️ Legal Notice

**This tool is for authorized security testing only.**

You must have explicit permission to test any bot tokens that you do not own. Unauthorized access is illegal.

## 🐛 Issues & Support

- **Report Issues**: https://github.com/kawaiipantsu/telegramdigger/issues
- **Email**: thugsred@protonmail.com

## 🙏 Credits

**Developer**: KawaiiPantsu
**Organization**: THUGS(red) Hacking Community

---

**Checksums** (SHA256):
```
# Run after download:
sha256sum telegramdigger_0.5.0_amd64.deb
```

Built with ❤️ for the security research community
