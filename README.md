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
- 👑 **Administrator Rights** - Read default bot administrator rights for groups and channels
- 🔗 **Webhook Management** - Get, set, and delete webhooks for bot updates
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

### Bulk Token Validation

Validate multiple tokens from a file or from previously seen tokens:

```bash
# Validate tokens from a custom file (one token per line)
telegramdigger --bulk-validate /path/to/tokens.txt

# Validate all previously seen tokens (uses ~/.telegramdigger/tokens-seen)
telegramdigger --bulk-validate
```

**Input File Format:**
- One token per line
- Comments start with `#`
- Automatically handles CSV format (token#date)
- Empty lines are ignored

**Example tokens.txt:**
```
# My bot tokens
123456789:ABCdefGHIjklMNOpqrsTUVwxyz-1234567890
987654321:XYZabcDEFghiJKLmnoPQRstuvWXYZ-0987654321
```

**Output Format:**
```
[1/3] 123456789:ABCde... - ✓ VALID - MyBot (@my_bot)
[2/3] 987654321:XYZab... - ✗ INVALID - HTTP request failed
[3/3] 111222333:TestT... - ✓ VALID - TestBot (@test_bot)

Summary:
──────────────────────────────────────────────────
Total tokens:    3
Valid tokens:    2
Invalid tokens:  1
```

### Bot Administrator Rights

Retrieve the default administrator rights configured for your bot:

```bash
# Read bot admin rights using command-line argument
telegramdigger --read-botrights --token "YOUR_BOT_TOKEN"

# Using environment variable
export TGDIGGER_TOKEN="YOUR_BOT_TOKEN"
telegramdigger --read-botrights

# Using config file
echo "bot_token=YOUR_BOT_TOKEN" >> ~/.telegramdigger/settings.conf
telegramdigger --read-botrights
```

**Output Format:**
```
 Reading Bot Default Administrator Rights
──────────────────────────────────────────────────

✓ Successfully retrieved bot rights

Groups & Supergroups:
──────────────────────────────────────────────────
Is Anonymous:           No
Can Manage Chat:        Yes
Can Delete Messages:    Yes
Can Manage Video Chats: Yes
Can Restrict Members:   Yes
Can Promote Members:    Yes
Can Change Info:        Yes
Can Invite Users:       Yes
Can Pin Messages:       Yes
Can Manage Topics:      Yes

Channels:
──────────────────────────────────────────────────
Is Anonymous:           No
Can Manage Chat:        Yes
Can Delete Messages:    Yes
Can Manage Video Chats: Yes
Can Restrict Members:   No
Can Promote Members:    Yes
Can Change Info:        Yes
Can Invite Users:       Yes
Can Post Messages:      Yes
Can Edit Messages:      Yes
Can Pin Messages:       Yes
Can Manage Topics:      No
```

### Webhook Management

Telegram bots can receive updates in two ways: long polling (getUpdates) or webhooks. Webhooks allow Telegram to push updates to your server via HTTPS POST requests.

#### Get Webhook Information

```bash
# Get current webhook status
telegramdigger --webhook-get --token "YOUR_BOT_TOKEN"

# Alias command
telegramdigger --webhook-info --token "YOUR_BOT_TOKEN"

# Using environment variable
export TGDIGGER_TOKEN="YOUR_BOT_TOKEN"
telegramdigger --webhook-get
```

**Output (No webhook set):**
```
 Webhook Information
──────────────────────────────────────────────────

✓ Successfully retrieved webhook information

Webhook Status:
──────────────────────────────────────────────────
No webhook is currently set
Use --webhook-set <URL> to configure a webhook
```

**Output (Webhook configured):**
```
 Webhook Information
──────────────────────────────────────────────────

✓ Successfully retrieved webhook information

Webhook Status:
──────────────────────────────────────────────────
URL:                    https://example.com/webhook
Custom Certificate:     No
Pending Updates:        0
IP Address:             203.0.113.42
Max Connections:        40
```

#### Set Webhook URL

```bash
# Set webhook to your server
telegramdigger --webhook-set https://example.com/webhook --token "YOUR_BOT_TOKEN"

# Using environment variable
export TGDIGGER_TOKEN="YOUR_BOT_TOKEN"
telegramdigger --webhook-set https://example.com/webhook
```

**Important Requirements:**
- URL must use HTTPS (required by Telegram)
- Must have a valid SSL certificate
- Must be publicly accessible from Telegram servers
- Should respond with HTTP 200 OK to POST requests

**Output:**
```
 Set Webhook
──────────────────────────────────────────────────

Setting webhook URL...
URL: https://example.com/webhook

✓ Webhook URL has been set successfully

Note: Make sure your webhook URL:
  - Uses HTTPS (required by Telegram)
  - Has a valid SSL certificate
  - Is publicly accessible
  - Responds with HTTP 200 OK to POST requests
```

#### Delete Webhook

```bash
# Remove webhook (switch to long polling)
telegramdigger --webhook-delete --token "YOUR_BOT_TOKEN"

# Using environment variable
export TGDIGGER_TOKEN="YOUR_BOT_TOKEN"
telegramdigger --webhook-delete
```

**Output:**
```
 Delete Webhook
──────────────────────────────────────────────────

Deleting webhook...

✓ Webhook has been deleted successfully

Your bot will no longer receive updates via webhook.
You can now use long polling (getUpdates) instead.
```

### Token Format

Telegram bot tokens follow the format: `<bot_id>:<token_hash>`

**Example:** `123456789:ABCdefGHIjklMNOpqrsTUVwxyz-1234567890`

### Command-Line Options

```bash
telegramdigger --help                    # Show help message
telegramdigger --version                 # Show version information
telegramdigger --about                   # Show about information
telegramdigger --token <TOKEN>           # Specify bot token
telegramdigger --validate                # Validate token via API
telegramdigger --bulk-validate [FILE]    # Bulk validate tokens from file
telegramdigger --read-botrights          # Read default bot admin rights
telegramdigger --webhook-get             # Get current webhook information
telegramdigger --webhook-info            # Get webhook status (alias)
telegramdigger --webhook-set <URL>       # Set webhook URL
telegramdigger --webhook-delete          # Delete webhook
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
