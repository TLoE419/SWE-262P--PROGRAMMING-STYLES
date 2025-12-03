import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;

/**
 * Week 9 Exercise - Style #28: Lazy Rivers using Iterators
 * This implementation uses custom Iterator classes to mimic Python generators
 */
public class Iterators {

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java Iterators <path_to_text_file>");
            System.exit(1);
        }

        String filename = args[0];

        try {
            // Create the processing pipeline using iterators
            Iterator<Map<String, Integer>> freqIterator = new CountAndSortIterator(filename);

            // Print results
            while (freqIterator.hasNext()) {
                Map<String, Integer> wordFreqs = freqIterator.next();
                System.out.println("-----------------------------");

                // Print top 25
                int count = 0;
                for (Map.Entry<String, Integer> entry : wordFreqs.entrySet()) {
                    if (count >= 25) break;
                    System.out.println(entry.getKey() + " - " + entry.getValue());
                    count++;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Iterator that yields characters from a file one at a time
     */
    static class CharacterIterator implements Iterator<Character> {
        private BufferedReader reader;
        private String currentLine;
        private int position;
        private Character nextChar;

        public CharacterIterator(String filename) throws IOException {
            this.reader = new BufferedReader(new FileReader(filename));
            this.currentLine = null;
            this.position = 0;
            advance();
        }

        private void advance() {
            try {
                // Try to get next character from current line
                if (currentLine != null && position < currentLine.length()) {
                    nextChar = currentLine.charAt(position++);
                    return;
                }

                // Need to read next line
                if (currentLine != null && position >= currentLine.length()) {
                    nextChar = '\n'; // Add newline at end of line
                    currentLine = null;
                    position = 0;
                    return;
                }

                // Read next line from file
                currentLine = reader.readLine();
                if (currentLine != null) {
                    position = 0;
                    advance();
                } else {
                    nextChar = null;
                    reader.close();
                }
            } catch (IOException e) {
                nextChar = null;
            }
        }

        @Override
        public boolean hasNext() {
            return nextChar != null;
        }

        @Override
        public Character next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }
            Character result = nextChar;
            advance();
            return result;
        }
    }

    /**
     * Iterator that yields words from character iterator
     */
    static class WordIterator implements Iterator<String> {
        private Iterator<Character> charIterator;
        private String nextWord;

        public WordIterator(String filename) throws IOException {
            this.charIterator = new CharacterIterator(filename);
            advance();
        }

        private void advance() {
            StringBuilder word = new StringBuilder();
            boolean inWord = false;

            while (charIterator.hasNext()) {
                char c = charIterator.next();

                if (Character.isLetterOrDigit(c)) {
                    word.append(Character.toLowerCase(c));
                    inWord = true;
                } else if (inWord) {
                    // End of word
                    nextWord = word.toString();
                    return;
                }
            }

            // Handle last word if file ends with alphanumeric
            if (inWord && word.length() > 0) {
                nextWord = word.toString();
            } else {
                nextWord = null;
            }
        }

        @Override
        public boolean hasNext() {
            return nextWord != null;
        }

        @Override
        public String next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }
            String result = nextWord;
            advance();
            return result;
        }
    }

    /**
     * Iterator that filters stop words
     */
    static class NonStopWordIterator implements Iterator<String> {
        private Iterator<String> wordIterator;
        private Set<String> stopWords;
        private String nextWord;

        public NonStopWordIterator(String filename) throws IOException {
            this.wordIterator = new WordIterator(filename);
            this.stopWords = loadStopWords();
            advance();
        }

        private Set<String> loadStopWords() throws IOException {
            Set<String> stops = new HashSet<>();

            // Load from stop_words.txt
            String content = new String(Files.readAllBytes(Paths.get("../stop_words.txt")));
            String[] words = content.split(",");
            for (String word : words) {
                stops.add(word.trim());
            }

            // Add single letters
            for (char c = 'a'; c <= 'z'; c++) {
                stops.add(String.valueOf(c));
            }

            return stops;
        }

        private void advance() {
            while (wordIterator.hasNext()) {
                String word = wordIterator.next();
                if (!stopWords.contains(word)) {
                    nextWord = word;
                    return;
                }
            }
            nextWord = null;
        }

        @Override
        public boolean hasNext() {
            return nextWord != null;
        }

        @Override
        public String next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }
            String result = nextWord;
            advance();
            return result;
        }
    }

    /**
     * Iterator that counts words and yields sorted frequency maps
     * Yields intermediate results every 5000 words
     */
    static class CountAndSortIterator implements Iterator<Map<String, Integer>> {
        private Iterator<String> wordIterator;
        private Map<String, Integer> frequencies;
        private int wordCount;
        private boolean hasYieldedFinal;

        public CountAndSortIterator(String filename) throws IOException {
            this.wordIterator = new NonStopWordIterator(filename);
            this.frequencies = new HashMap<>();
            this.wordCount = 0;
            this.hasYieldedFinal = false;
        }

        @Override
        public boolean hasNext() {
            // Process words until we hit a 5000 boundary or end of input
            while (wordIterator.hasNext()) {
                String word = wordIterator.next();
                frequencies.put(word, frequencies.getOrDefault(word, 0) + 1);
                wordCount++;

                if (wordCount % 5000 == 0) {
                    return true; // Ready to yield intermediate result
                }
            }

            // Yield final result if we haven't yet
            return !hasYieldedFinal;
        }

        @Override
        public Map<String, Integer> next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }

            // Sort frequencies and return as LinkedHashMap to preserve order
            Map<String, Integer> sorted = frequencies.entrySet()
                .stream()
                .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
                .collect(LinkedHashMap::new,
                        (map, entry) -> map.put(entry.getKey(), entry.getValue()),
                        LinkedHashMap::putAll);

            // Check if this is the final result
            if (!wordIterator.hasNext()) {
                hasYieldedFinal = true;
            }

            return sorted;
        }
    }
}
