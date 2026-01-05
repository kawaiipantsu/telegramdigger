# GitHub Release Guide for v0.5.0

This guide explains how to create the v0.5.0 release on GitHub.

## Release Files

The following files should be included in the release:

### 📦 Binary Package
- `telegramdigger_0.5.0_amd64.deb` (50 KB) - Debian/Ubuntu package

### 📄 Documentation
- `RELEASE_v0.5.0.md` - Release notes for GitHub
- `SHA256SUMS` - Checksum file for verification

## Creating the Release on GitHub

### 1. Navigate to Releases
Go to: https://github.com/kawaiipantsu/telegramdigger/releases/new

### 2. Tag Information
- **Tag version**: `v0.5.0`
- **Target**: `main` (or your default branch)
- **Release title**: `TelegramDigger v0.5.0 - Full-Featured Bot Token Analysis Tool`

### 3. Release Description

Copy the content from `RELEASE_v0.5.0.md` into the release description field.

### 4. Upload Assets

Drag and drop or upload these files:
1. `telegramdigger_0.5.0_amd64.deb`
2. `SHA256SUMS`

### 5. Additional Settings

- ☑️ Set as the latest release
- ☐ Set as a pre-release (leave unchecked)
- ☐ Create a discussion for this release (optional)

### 6. Publish

Click "Publish release" to make it live.

## Post-Release

After publishing, verify:
1. Release appears at https://github.com/kawaiipantsu/telegramdigger/releases
2. Assets are downloadable
3. Checksums match:
   ```bash
   wget https://github.com/kawaiipantsu/telegramdigger/releases/download/v0.5.0/telegramdigger_0.5.0_amd64.deb
   wget https://github.com/kawaiipantsu/telegramdigger/releases/download/v0.5.0/SHA256SUMS
   sha256sum -c SHA256SUMS
   ```

## Update Documentation

After release, consider updating:
- README.md installation section with new download links
- Project description to mention latest version

## Social Media / Announcement Template

```
🎉 TelegramDigger v0.5.0 Released!

Full-featured security testing tool for Telegram bot tokens:
🔗 Webhook management
👑 Bot administrator rights
📊 Bulk token validation
🔐 Security hardened

Download: https://github.com/kawaiipantsu/telegramdigger/releases/tag/v0.5.0

#infosec #pentest #telegram #security #ctf
```

## Git Commands (If Using Git)

If this is a git repository, tag the release locally:

```bash
# Create annotated tag
git tag -a v0.5.0 -m "Release version 0.5.0 - Full-Featured Bot Token Analysis Tool"

# Push tag to GitHub
git push origin v0.5.0

# Or push all tags
git push --tags
```

## Release Checklist

- [x] Version updated in `include/version.h` (0.5.0)
- [x] Version updated in `Makefile` (0.5.0)
- [x] Project compiled successfully
- [x] DEB package built (telegramdigger_0.5.0_amd64.deb)
- [x] CHANGELOG.md created with full history
- [x] RELEASE_NOTES.md created
- [x] RELEASE_v0.5.0.md created for GitHub
- [x] SHA256 checksums generated
- [ ] Git tag created (if using git)
- [ ] GitHub release created
- [ ] Release assets uploaded
- [ ] Release published
- [ ] Downloads tested
- [ ] README.md updated with download links

## Support Information

After release, users can get support via:
- GitHub Issues: https://github.com/kawaiipantsu/telegramdigger/issues
- Email: thugsred@protonmail.com
- Community: THUGS(red) Hacking Community

---

**Prepared by**: KawaiiPantsu
**Date**: January 5, 2026
**Organization**: THUGS(red) Hacking Community
