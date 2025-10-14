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
    
for b_tag in soup.find_all('b'):
    b_tag.name = "blockquote"

with open(filename, "w", encoding="utf-8") as f:
    f.write(soup.prettify())
    