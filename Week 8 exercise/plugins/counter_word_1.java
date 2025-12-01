package plugins;

import java.util.*;

/**
 * Plugin 1: Normal word counting - counts word frequencies and returns top 25
 */
public class counter_word_1 {

    public Map<String, Integer> count(List<String> words) {
        // Count word frequencies
        Map<String, Integer> wordFreqs = new HashMap<>();
        for (String word : words) {
            wordFreqs.put(word, wordFreqs.getOrDefault(word, 0) + 1);
        }

        // Sort by frequency (descending) and return as LinkedHashMap to preserve order
        List<Map.Entry<String, Integer>> sortedEntries = new ArrayList<>(wordFreqs.entrySet());
        sortedEntries.sort((e1, e2) -> e2.getValue().compareTo(e1.getValue()));

        // Use LinkedHashMap to maintain insertion order (sorted order)
        Map<String, Integer> result = new LinkedHashMap<>();
        for (Map.Entry<String, Integer> entry : sortedEntries) {
            result.put(entry.getKey(), entry.getValue());
        }

        return result;
    }
}
