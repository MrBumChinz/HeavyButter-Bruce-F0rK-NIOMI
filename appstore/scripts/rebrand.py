#!/usr/bin/env python3
"""Rebrand App Store catalog apps from 'Bruce' to 'SharkSoup HeavyButter Edition'.

Usage: python3 rebrand.py <www_dir>

Modifies category-*.min.json files in place:
  - App names (n): "Bruce" -> "SharkSoup"
  - Descriptions (d): Rewrites Bruce references to our branding
  - Keeps source paths (s) intact (they point to real upstream repos)
"""

import json
import os
import re
import sys

def rebrand_app(app):
    """Rebrand a single app entry."""
    name = app.get("n", "")
    desc = app.get("d", "")
    original_name = name
    original_desc = desc

    # ── Rebrand name ──────────────────────────────────────────
    # "Bruce Theme" -> "SharkSoup Theme"
    # "Bruce <something>" -> "SharkSoup <something>"
    name = re.sub(r'\bBruce\b', 'SharkSoup', name)
    # "Bruce" as standalone -> "SharkSoup"
    name = name.replace('Bruce', 'SharkSoup')

    # ── Rebrand description ───────────────────────────────────
    # "your Bruce device" -> "your SharkSoup device"
    desc = re.sub(r'\byour Bruce\b', 'your SharkSoup', desc)
    desc = re.sub(r'\bYour Bruce\b', 'Your SharkSoup', desc)
    # "for Bruce" -> "for SharkSoup HeavyButter"
    desc = re.sub(r'\bfor Bruce\b', 'for SharkSoup HeavyButter', desc)
    # "Bruce device" -> "SharkSoup HeavyButter device"
    desc = re.sub(r'\bBruce device\b', 'SharkSoup HeavyButter device', desc)
    # "Bruce firmware" -> "SharkSoup HeavyButter firmware"
    desc = re.sub(r'\bBruce firmware\b', 'SharkSoup HeavyButter firmware', desc)
    desc = re.sub(r'\bBruce Firmware\b', 'SharkSoup HeavyButter Firmware', desc)
    # "Bruce's" -> "SharkSoup's"
    desc = re.sub(r"\bBruce's\b", "SharkSoup's", desc)
    # Any remaining standalone "Bruce" references
    desc = re.sub(r'\bBruce\b', 'SharkSoup HeavyButter', desc)

    # ── Fix awkward double-branding ───────────────────────────
    # "SharkSoup HeavyButter HeavyButter" -> "SharkSoup HeavyButter"
    desc = desc.replace('HeavyButter HeavyButter', 'HeavyButter')
    desc = desc.replace('SharkSoup SharkSoup', 'SharkSoup')

    if name != original_name or desc != original_desc:
        app["n"] = name
        app["d"] = desc
        return True
    return False


def rebrand_category(cat_file):
    """Rebrand all apps in a category file. Returns number of apps modified."""
    with open(cat_file, 'r') as f:
        data = json.load(f)

    count = 0
    for app in data.get('apps', []):
        if rebrand_app(app):
            count += 1

    # Also rebrand the category name if it mentions Bruce
    if 'category' in data:
        cat_name = data['category']
        new_cat = cat_name.replace('Bruce', 'SharkSoup')
        if new_cat != cat_name:
            data['category'] = new_cat

    with open(cat_file, 'w') as f:
        json.dump(data, f, separators=(',', ':'))
    return count


def main():
    if len(sys.argv) < 2:
        print("Usage: python3 rebrand.py <www_dir>")
        sys.exit(1)

    www_dir = sys.argv[1]
    releases_dir = os.path.join(www_dir, 'service', 'main', 'releases')

    if not os.path.isdir(releases_dir):
        print(f"Error: {releases_dir} not found")
        sys.exit(1)

    total_modified = 0
    total_files = 0

    for fname in sorted(os.listdir(releases_dir)):
        if fname.startswith('category-') and fname.endswith('.min.json'):
            fpath = os.path.join(releases_dir, fname)
            modified = rebrand_category(fpath)
            if modified > 0:
                print(f"  {fname}: {modified} apps rebranded")
            total_files += 1
            total_modified += modified

    print(f"\nRebranded {total_modified} apps across {total_files} categories.")

if __name__ == '__main__':
    main()
