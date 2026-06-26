# GitHub setup guide

## Recommended repository name

`business-account-contact-manager`

## Repository title

Business Account & Contact Manager (BACM System)

## Repository description

Console-based C++ account and contact manager with CSV persistence, hash-table lookup, BST sorting, pagination, undo support, and benchmark testing.

## Suggested visibility

Use `Public` if this is for a portfolio/class showcase. Use `Private` if the repository may contain real customer/contact data.

## Suggested topics

```text
cpp cpp20 console-application csv data-structures binary-search-tree hash-table stack account-management contact-management visual-studio cmake
```

## Commands to create the repository locally

From inside this folder:

```bash
git init
git add .
git commit -m "Initial commit: add BACM System"
git branch -M main
```

Then create an empty GitHub repository named `business-account-contact-manager` and connect it:

```bash
git remote add origin https://github.com/YOUR-USERNAME/business-account-contact-manager.git
git push -u origin main
```

## What to upload

Upload the cleaned repository contents, not the original ZIP. The original ZIP contains Visual Studio cache files and build artifacts that should stay out of GitHub.
