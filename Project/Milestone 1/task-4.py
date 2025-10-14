import sys
from bs4 import BeautifulSoup

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

# Single API call to find all tags with id attribute
for tag in soup.find_all(id=True):
    print(tag)