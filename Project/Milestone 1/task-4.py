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
    
for tag in soup.find_all():
    if(tag.has_attr("id")):
        print(tag)

# ask output format. Is those tags inside the id tag need to display on terminal?