package plugins;

import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;

/**
 * Plugin 2: Extract only non-stop words that contain the letter 'z'
 */
public class extractor_word_2 {

    public List<String> extractWords(String pathToFile) {
        List<String> words = new ArrayList<>();
        Set<String> stopWords = loadStopWords();

        try {
            String content = new String(Files.readAllBytes(Paths.get(pathToFile)));
            content = content.toLowerCase();

            // Extract words (alphanumeric sequences of 2+ characters)
            Pattern pattern = Pattern.compile("[a-z]{2,}");
            Matcher matcher = pattern.matcher(content);

            while (matcher.find()) {
                String word = matcher.group();
                // Only include words that contain 'z' and are not stop words
                if (!stopWords.contains(word) && word.contains("z")) {
                    words.add(word);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return words;
    }

    private Set<String> loadStopWords() {
        Set<String> stopWords = new HashSet<>();
        try {
            String content = new String(Files.readAllBytes(Paths.get("../stop_words.txt")));
            String[] words = content.split(",");
            for (String word : words) {
                stopWords.add(word.trim());
            }
            // Add single letters
            for (char c = 'a'; c <= 'z'; c++) {
                stopWords.add(String.valueOf(c));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return stopWords;
    }
}
