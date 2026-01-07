# Message Formatting Guide

TelegramDigger supports three formatting modes for sending messages: Markdown, MarkdownV2, and HTML.

## Usage

```bash
telegramdigger --send-message "Your formatted text" --chatid CHAT_ID [OPTIONS]
```

### Available Options

- `--parse-mode MODE` - Formatting mode: `Markdown`, `MarkdownV2`, or `HTML`
- `--silent` - Send message silently (no notification to users)
- `--nopreview` - Disable link preview for URLs in the message

## Markdown Mode

### Supported Formatting

| Style | Syntax | Example |
|-------|--------|---------|
| Bold | `*text*` | `*bold text*` |
| Italic | `_text_` | `_italic text_` |
| Inline Code | `` `text` `` | `` `inline code` `` |
| Pre-formatted Code | ``` ```text``` ``` | ``` ```code block``` ``` |
| Inline Link | `[text](URL)` | `[Google](https://google.com)` |

### Example

```bash
telegramdigger --send-message "*Bold* _italic_ `code` [link](https://example.com)" \
  --chatid 123456789 --parse-mode Markdown
```

## MarkdownV2 Mode

MarkdownV2 is more strict and requires escaping special characters.

### Supported Formatting

| Style | Syntax | Example |
|-------|--------|---------|
| Bold | `*text*` | `*bold*` |
| Italic | `_text_` | `_italic_` |
| Underline | `__text__` | `__underline__` |
| Strikethrough | `~text~` | `~strikethrough~` |
| Spoiler | `\|\|text\|\|` | `\|\|spoiler\|\|` |
| Inline Code | `` `text` `` | `` `code` `` |
| Pre-formatted | ``` ```text``` ``` | ``` ```block``` ``` |
| Inline Link | `[text](URL)` | `[link](https://example.com)` |

### Characters that need escaping
`_`, `*`, `[`, `]`, `(`, `)`, `~`, `` ` ``, `>`, `#`, `+`, `-`, `=`, `|`, `{`, `}`, `.`, `!`

### Example

```bash
telegramdigger --send-message "*bold* _italic_ __underline__ ~strike~ \`code\`" \
  --chatid 123456789 --parse-mode MarkdownV2
```

## HTML Mode

### Supported Tags

| Style | Tag | Example |
|-------|-----|---------|
| Bold | `<b>` or `<strong>` | `<b>bold text</b>` |
| Italic | `<i>` or `<em>` | `<i>italic text</i>` |
| Underline | `<u>` | `<u>underlined text</u>` |
| Strikethrough | `<s>` or `<strike>` or `<del>` | `<s>strikethrough</s>` |
| Spoiler | `<tg-spoiler>` or `<span class="tg-spoiler">` | `<tg-spoiler>spoiler</tg-spoiler>` |
| Inline Link | `<a href="URL">` | `<a href="https://example.com">link</a>` |
| Inline Code | `<code>` | `<code>inline code</code>` |
| Pre-formatted | `<pre>` | `<pre>code block</pre>` |
| Pre-formatted with language | `<pre><code class="language-*">` | `<pre><code class="language-python">code</code></pre>` |

### Example

```bash
telegramdigger --send-message "<b>Bold</b> <i>italic</i> <u>underline</u> <code>code</code>" \
  --chatid 123456789 --parse-mode HTML
```

## Advanced Examples

### Multi-line Message with Markdown

```bash
telegramdigger --send-message "
*Security Alert*

_Bot Token Detected:_ \`123456:ABC...\`

*Recommendations:*
• Rotate the token immediately
• Check access logs
• Update security policies
" --chatid 123456789 --parse-mode Markdown
```

### HTML with Code Block

```bash
telegramdigger --send-message "
<b>Configuration Found:</b>

<pre><code class=\"language-json\">
{
  \"api_key\": \"secret\",
  \"enabled\": true
}
</code></pre>

<i>Review and secure immediately.</i>
" --chatid 123456789 --parse-mode HTML
```

### Markdown with Links

```bash
telegramdigger --send-message "
*Alert:* Exposed credentials detected

[View Report](https://report.example.com)
[Documentation](https://docs.example.com)
" --chatid 123456789 --parse-mode Markdown
```

## Special Options

### Silent Messages (`--silent`)

Send messages without triggering notifications for recipients. Useful for:
- Low-priority alerts that don't need immediate attention
- Automated logs or status updates
- Night-time messages that shouldn't wake users

```bash
telegramdigger --send-message "Low priority update" --chatid 123 --silent
```

### Disable Link Preview (`--nopreview`)

Prevent Telegram from generating link previews. Useful for:
- Messages with multiple links (keeps it clean)
- Security alerts with potentially dangerous URLs
- When you want to save bandwidth

```bash
telegramdigger --send-message "Check https://example.com" --chatid 123 --nopreview
```

### Combining Options

You can combine all options together:

```bash
telegramdigger --send-message "<b>Alert:</b> Token found at <code>https://github.com/...</code>" \
  --chatid 123 --parse-mode HTML --silent --nopreview
```

## Tips

1. **Choose the right mode:**
   - Use `Markdown` for simple formatting (easiest)
   - Use `HTML` for complex formatting with nested tags
   - Use `MarkdownV2` for advanced Markdown features (requires escaping)

2. **Use silent mode wisely:**
   - For automated monitoring that runs frequently
   - For non-urgent security findings
   - For informational messages that don't need immediate action

3. **Disable previews when:**
   - Sending security alerts with suspicious URLs
   - Message contains many links (cleaner appearance)
   - You want faster message delivery

4. **Test your formatting:**
   - Always test with a private chat first
   - Check how special characters are displayed
   - Verify links work correctly

5. **Error handling:**
   - Invalid formatting will cause the API to reject the message
   - The error will be displayed with details
   - Fix the formatting and try again

6. **Plain text:**
   - Omit `--parse-mode` to send plain text
   - Special characters won't be interpreted

## Common Use Cases

### Security Alerts

```bash
# HTML format for clear alerts (with silent mode for off-hours)
telegramdigger --send-message "
<b>🔴 SECURITY ALERT</b>

<b>Issue:</b> Exposed API token
<b>Severity:</b> Critical
<b>Location:</b> Production server

<i>Action required immediately</i>
" --chatid -1001234567890 --parse-mode HTML --silent
```

```bash
# Alert with URL but no preview (security best practice)
telegramdigger --send-message "
<b>⚠️ Token Exposed</b>

Found at: <code>https://github.com/repo/file.js</code>

Review immediately.
" --chatid 123456789 --parse-mode HTML --nopreview
```

### Bot Information Summary

```bash
# Markdown for clean summaries
telegramdigger --send-message "
*Bot Validation Complete*

_Bot Name:_ MyBot
_Username:_ @mybot
_Status:_ ✅ Active

\`Token saved to database\`
" --chatid 123456789 --parse-mode Markdown
```

### Code Snippets

```bash
# HTML with syntax highlighting
telegramdigger --send-message "
Found configuration file:

<pre><code class=\"language-python\">
API_TOKEN = '123456:ABC...'
WEBHOOK_URL = 'https://example.com'
DEBUG = True
</code></pre>
" --chatid 123456789 --parse-mode HTML
```

### Monitoring and Automation

```bash
# Silent monitoring updates (won't disturb users)
telegramdigger --send-message "
*Hourly Status Report*

• Tokens validated: 15
• New findings: 2
• All systems operational

_Last check: $(date)_
" --chatid 123456789 --parse-mode Markdown --silent
```

```bash
# Bulk notifications with links (no preview to keep clean)
telegramdigger --send-message "
*Daily Summary*

Reports available:
• Security: https://report.example.com/security
• Performance: https://report.example.com/perf
• Logs: https://report.example.com/logs
" --chatid 123 --parse-mode Markdown --nopreview
```
