import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;
import java.util.function.*;

/**
 * Week 9 Exercise - Style #28: Lazy Rivers using Java Stream API
 * This implementation uses Java 8+ Stream API for lazy evaluation
 */
public class Streams {

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java Streams <path_to_text_file>");
            System.exit(1);
        }

        String filename = args[0];

        try {
            // Load stop words
            Set<String> stopWords = loadStopWords();

            // Create a stream pipeline that processes the file lazily
            Stream<String> words = Files.lines(Paths.get(filename))
                .flatMap(line -> Arrays.stream(line.split("[^a-zA-Z0-9]+")))
                .map(String::toLowerCase)
                .filter(word -> !word.isEmpty())
                .filter(word -> !stopWords.contains(word));

            // Count frequencies with intermediate results every 5000 words
            countAndPrintWithProgress(words);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Load stop words from file
     */
    private static Set<String> loadStopWords() throws IOException {
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

    /**
     * Count word frequencies and print intermediate results every 5000 words
     * This uses a custom collector to achieve the lazy behavior
     */
    private static void countAndPrintWithProgress(Stream<String> words) {
        Map<String, Integer> frequencies = new HashMap<>();
        final int[] count = {0};

        words.forEach(word -> {
            frequencies.put(word, frequencies.getOrDefault(word, 0) + 1);
            count[0]++;

            // Print intermediate results every 5000 words
            if (count[0] % 5000 == 0) {
                printTopWords(frequencies);
            }
        });

        // Print final results
        printTopWords(frequencies);
    }

    /**
     * Print top 25 words by frequency
     */
    private static void printTopWords(Map<String, Integer> frequencies) {
        System.out.println("-----------------------------");

        frequencies.entrySet()
            .stream()
            .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
            .limit(25)
            .forEach(entry -> System.out.println(entry.getKey() + " - " + entry.getValue()));
    }

    /**
     * Alternative pure stream implementation (no intermediate results)
     * Uncomment the method below and call it from main if you want
     * a completely functional stream-based approach
     */
    /*
    private static void pureStreamApproach(String filename) throws IOException {
        Set<String> stopWords = loadStopWords();

        Map<String, Long> wordFrequencies = Files.lines(Paths.get(filename))
            .flatMap(line -> Arrays.stream(line.split("[^a-zA-Z0-9]+")))
            .map(String::toLowerCase)
            .filter(word -> !word.isEmpty())
            .filter(word -> !stopWords.contains(word))
            .collect(Collectors.groupingBy(
                Function.identity(),
                Collectors.counting()
            ));

        System.out.println("-----------------------------");
        wordFrequencies.entrySet()
            .stream()
            .sorted(Map.Entry.<String, Long>comparingByValue().reversed())
            .limit(25)
            .forEach(entry -> System.out.println(entry.getKey() + " - " + entry.getValue()));
    }
    */
}
