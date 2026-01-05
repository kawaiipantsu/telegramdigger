# TelegramDigger

```
       ___/\   TELEGRAM DIGGER by THUGSred
      /     \  Security Testing Tool for Telegram Bot Tokens
   ___.#===___
  /..#####.###
 /###.######.###\
 \###..####.##./
```

A professional C++ security testing tool for analyzing and validating Telegram bot tokens discovered during penetration testing and security assessments.

**Developed by KawaiiPantsu**
*Member of THUGS(red) Hacking Community*
*Ethical Hacker & Security Researcher*

---

## Features

- 🔍 **Token Validation** - Validate bot tokens via Telegram Bot API
- 📊 **Bot Information Extraction** - Retrieve detailed bot capabilities and permissions
- 💾 **Token Tracking** - Automatic storage of validated tokens and metadata
- 🎨 **Styled Terminal Output** - ANSI 256-color support with UTF-8 icons
- 🔐 **Security Hardened** - Built with stack protection, PIE, and secure permissions
- 📦 **DEB Packaging** - Easy installation on Debian-based systems

## Installation

### Prerequisites

**Debian/Ubuntu:**
```bash
sudo apt-get install build-essential dpkg-dev libcurl4-openssl-dev
```

### Build from Source

```bash
# Clone the repository
git clone https://github.com/kawaiipantsu/telegramdigger.git
cd telegramdigger

# Build
make

# Install system-wide (optional)
sudo make install

# Or build DEB package
make deb
sudo dpkg -i telegramdigger_*.deb
```

## Usage

### Basic Token Validation

```bash
# Validate using command-line argument
telegramdigger --validate --token "YOUR_BOT_TOKEN"

# Using environment variable
export TGDIGGER_TOKEN="YOUR_BOT_TOKEN"
telegramdigger --validate

# Using config file
echo "bot_token=YOUR_BOT_TOKEN" >> ~/.telegramdigger/settings.conf
telegramdigger --validate
```

### Token Format

Telegram bot tokens follow the format: `<bot_id>:<token_hash>`

**Example:** `123456789:ABCdefGHIjklMNOpqrsTUVwxyz-1234567890`

### Command-Line Options

```bash
telegramdigger --help              # Show help message
telegramdigger --version           # Show version information
telegramdigger --about             # Show about information
telegramdigger --token <TOKEN>     # Specify bot token
telegramdigger --validate          # Validate token via API
```

### Environment Variables

- `TGDIGGER_TOKEN` - Bot token (alternative to --token flag)

## Output Example

```
       ___/\   TELEGRAM DIGGER by THUGSred
      /     \  Security Testing Tool for Telegram Bot Tokens
   ___.#===___
  /..#####.###
 /###.######.###\
 \###..####.##./

 Validating Bot Token
──────────────────────────────────────────────────

Token format: ✓ Valid

Connecting to Telegram API...
Calling getMe endpoint...

✓ Token is valid!

Bot Information:
──────────────────────────────────────────────────
Bot ID:           123456789
Bot Name:         MyTestBot
Username:         @my_test_bot
Is Bot:           Yes

Capabilities:
──────────────────────────────────────────────────
Can Join Groups:         Yes
Can Read All Messages:   No
Supports Inline Queries: Yes

 Token info saved to: /root/.telegramdigger/valid-tokens/123456789:ABC...
✓ Token added to seen list
```

## Token Storage

When a token is successfully validated, TelegramDigger automatically saves:

### Valid Token Information
**Location:** `~/.telegramdigger/valid-tokens/<token>`

Contains:
- Bot ID, name, and username
- Bot capabilities and permissions
- Validation timestamp

**Permissions:** `0600` (owner read/write only)

### Token History
**Location:** `~/.telegramdigger/tokens-seen`

CSV format: `<token>#<timestamp>`
```
123456789:ABCdefGHI...#2026-01-05 12:34:56
987654321:XYZabcdef...#2026-01-05 14:22:10
```

## Configuration

Configuration directory: `~/.telegramdigger/`

### Structure
```
~/.telegramdigger/
├── settings.conf           # User configuration (key=value format)
├── tokens-seen            # CSV log of validated tokens
└── valid-tokens/          # Directory containing bot information
    ├── <token1>           # Bot details for token1
    └── <token2>           # Bot details for token2
```

### settings.conf Format
```ini
# Configuration file for TelegramDigger
bot_token=123456789:ABCdefGHI...
# Add more settings as needed
```

## Security Considerations

### Build Security Features
- Stack buffer overflow protection (`-fstack-protector-strong`)
- Position Independent Executable (PIE)
- Read-only relocations (`RELRO`)
- Format string vulnerability detection
- Fortified source compilation

### File Permissions
- Config directory: `0700` (owner only)
- Config files: `0600` (owner read/write only)
- Token storage: `0600` (owner read/write only)

### HTTPS Communication
- All API requests use HTTPS
- SSL certificate verification enabled by default
- Secure communication with Telegram servers

## Development

### Build System
- **Language:** C++17
- **Build Tool:** GNU Make
- **Dependencies:** libcurl4-openssl-dev

### Build Commands
```bash
make                # Build the application
make clean          # Remove build artifacts
make install        # Install to /usr/local/bin
make deb            # Create Debian package
make help           # Show all targets
```

### Project Structure
```
telegramdigger/
├── src/              # Source files
│   ├── main.cpp           # Entry point and CLI
│   ├── terminal.cpp       # Terminal styling
│   ├── config.cpp         # Configuration manager
│   ├── http_client.cpp    # HTTP/HTTPS client
│   └── telegram_api.cpp   # Telegram API client
├── include/          # Header files
├── Makefile          # Build system
└── README.md         # This file
```

## Legal Notice

⚠️ **IMPORTANT:** This tool is intended for:
- Authorized security testing and penetration testing
- CTF (Capture The Flag) challenges
- Educational and research purposes
- Defensive security analysis

**You must have explicit permission to test any bot tokens that you do not own.**

Unauthorized access to computer systems is illegal. Use this tool responsibly and ethically.

## Contributing

This project is maintained by KawaiiPantsu and the THUGS(red) Hacking Community.

Contributions, bug reports, and feature requests are welcome!

## License

See the project repository for license information.

## Contact & Support

**For issues and questions:**
- GitHub Issues: https://github.com/kawaiipantsu/telegramdigger/issues
- Email: thugsred@protonmail.com
- Community: THUGS(red) Hacking Community

**Project Maintainer:** KawaiiPantsu (THUGSred member)

---

**Developed by KawaiiPantsu | THUGS(red) Hacking Community**
*Made with ❤️ for the security research community*
