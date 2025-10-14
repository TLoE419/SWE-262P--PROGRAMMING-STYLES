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
    
with open(filename, 'w', encoding='utf-8') as f:
    f.write(soup.prettify())

# Is write back on disk means write back to file
# When I write tree back to a file, the original indentation gets messed up.