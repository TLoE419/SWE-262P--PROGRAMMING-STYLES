import sys
from bs4 import BeautifulSoup
import os

if len(sys.argv) != 2:
    print(f"Without input file")
    sys.exit(1)

filename = sys.argv[1]

try:
    with open(filename, 'r', encoding='utf-8') as f:
        soup = BeautifulSoup(f, 'html.parser')
except FileNotFoundError:
    print(f"File {filename} not found.")
    sys.exit(1)

base_name, ext = os.path.splitext(filename)
output_filename = f"{base_name}-prettified{ext}"

with open(output_filename, 'w', encoding='utf-8') as f:
    f.write(soup.prettify())
