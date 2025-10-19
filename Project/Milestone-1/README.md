# Milestone 1 - BeautifulSoup HTML/XML Parsing

This milestone demonstrates various BeautifulSoup API operations for HTML/XML parsing and manipulation.

## Requirements

```bash
pip install beautifulsoup4
```

## Tasks

### Task 1: Prettify HTML/XML
Read an HTML/XML file and write the tree back as a prettified file.

```bash
python task-1.py <input-file>
```

### Task 2: Extract Hyperlinks
Print out all the hyperlinks (`<a>` tags).

```bash
python task-2.py <input-file>
```

### Task 3: Print All Tags
Print out all the tags in the document.

```bash
python task-3.py <input-file>
```

### Task 4: Find Tags with ID Attribute
Print all tags that have an `id` attribute (using a single API call).

```bash
python task-4.py <input-file>
```

### Task 5: Use find_parent()
Find all `<a>` tags and print their parent `<div>` attributes.

```bash
python task-5.py <input-file>
```

### Task 6: Change Tags
Change all `<b>` tags to `<blockquote>` tags and write to file.

```bash
python task-6.py <input-file>
```

### Task 7: Add Class Attributes
Find all `<p>` tags and add/replace class attribute with `class="test"`.

```bash
python task-7.py <input-file>
```

### Task 8: Additional API Functions
Create a new `<div>` tag and insert it after the first `<p>` tag.

```bash
python task-8.py <input-file>
```

## API Functions Used

| Task | BeautifulSoup Functions |
|------|------------------------|
| 1 | `prettify()` |
| 2 | `find_all('a')`, attribute access |
| 3 | `find_all()` |
| 4 | `find_all(id=True)` |
| 5 | `find_parent()` |
| 6 | `tag.name` modification |
| 7 | `tag['class']` assignment |
| 8 | `new_tag()`, `insert_after()` |
