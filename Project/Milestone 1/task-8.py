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
    
p1_tag = soup.find('p')
new_div = soup.new_tag("div")
new_div.string = "New div"

p1_tag.insert_after(new_div)

print(soup.prettify())

with open(filename, "w", encoding="utf-8") as f:
    f.write(soup.prettify())
    
