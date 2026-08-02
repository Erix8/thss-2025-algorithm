#!/usr/bin/env python3
"""
Generate test data files for StringMatching benchmarks.

Four scenarios designed to highlight the differences between:
  - BruteForce (O(n·m))
  - KMP (O(n), excels with repetitive/self-similar patterns)
  - Boyer-Moore (O(n/m) avg, excels with long patterns + large alphabets)
"""

import os
import random
import string

OUTDIR = "data"
os.makedirs(OUTDIR, exist_ok=True)


def write_files(tag, text, pattern):
    """Write text and pattern to files named <tag>_text.txt and <tag>_pattern.txt."""
    tpath = os.path.join(OUTDIR, f"{tag}_text.txt")
    ppath = os.path.join(OUTDIR, f"{tag}_pattern.txt")
    with open(tpath, "w") as f:
        f.write(text)
    with open(ppath, "w") as f:
        f.write(pattern)
    print(f"  Wrote {tag}: text={len(text):,} chars, pattern={len(pattern)} chars")


# ============================================================
# Scenario A: Near-matches — KMP excels
# Text: 100K 'A's.  Pattern: "AAAA...AB" (4 As + B).
# BruteForce compares 4 matching chars at every position then fails on B.
# KMP uses LPS to skip the redundant re-comparison.
# ============================================================
print("=== Scenario A: Repetitive near-matches (KMP advantage) ===")
text_a = "A" * 100_000
pattern_a = "A" * 4 + "B"
write_files("scenarioA", text_a, pattern_a)

# Also test with longer repeated prefix for bigger gap
pattern_a2 = "A" * 40 + "B"
write_files("scenarioA_long", text_a, pattern_a2)

# ============================================================
# Scenario B: Long pattern + large alphabet — Boyer-Moore excels
# Text: 1M random chars from [a-zA-Z0-9] (62-char alphabet).
# Pattern: long unique string, appears 0-1 times.
# BM's bad-char rule: most text chars not in pattern → shift by m.
# ============================================================
print("=== Scenario B: Long pattern + large alphabet (BM advantage) ===")
ALPHABET62 = string.ascii_letters + string.digits  # 62 chars
random.seed(42)
text_b = "".join(random.choices(ALPHABET62, k=1_000_000))

# Insert exactly 1 occurrence at position 500000 so answers can be verified
# (and so it's not trivial "no match" case)
for pattern_len in [10, 30, 60, 120, 240]:
    tag = f"scenarioB_m{pattern_len}"
    # Generate a unique pattern that doesn't appear by chance
    pattern_b = "".join(random.choices(ALPHABET62, k=pattern_len))
    # Embed it at position 500000
    text_with_match = text_b[:500_000] + pattern_b + text_b[500_000 + pattern_len:]
    write_files(tag, text_with_match, pattern_b)

# ============================================================
# Scenario C: Varying pattern length on same 500K text
# Fixed text, pattern lengths: 5, 10, 20, 50, 100, 200, 400
# Shows how BM's O(n/m) advantage grows with m
# ============================================================
print("=== Scenario C: Pattern length sweep (fixed text) ===")
random.seed(123)
text_c = "".join(random.choices(string.ascii_lowercase, k=500_000))

for pattern_len in [5, 10, 20, 50, 100, 200, 400]:
    tag = f"scenarioC_m{pattern_len}"
    # Embed at a fixed position so answers are consistent
    pos = 250_000
    pattern_c = "".join(random.choices(string.ascii_lowercase, k=pattern_len))
    text_with = text_c[:pos] + pattern_c + text_c[pos + pattern_len:]
    write_files(tag, text_with, pattern_c)

# ============================================================
# Scenario D: Worst-case boundary (last char mismatch)
# Text: "ABCDE" repeated 100K times. Pattern: "ABCDZ"
# Every position: compare 4 chars ok, fail at 5th. BruteForce worst case.
# Both KMP and BM handle this efficiently.
# ============================================================
print("=== Scenario D: Worst-case last-char mismatch ===")
text_d = "ABCDE" * 100_000
pattern_d = "ABCDZ"
write_files("scenarioD", text_d, pattern_d)

print(f"\nAll test data generated in '{OUTDIR}/'")