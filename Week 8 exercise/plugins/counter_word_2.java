package plugins;

import java.util.*;

/**
 * Plugin 2: Count words by first letter - groups words by their starting letter
 */
public class counter_word_2 {

    public Map<String, Integer> count(List<String> words) {
        // Count words by their first letter
        Map<String, Integer> letterCounts = new HashMap<>();

        for (String word : words) {
            if (word.length() > 0) {
                String firstLetter = String.valueOf(word.charAt(0));
                letterCounts.put(firstLetter, letterCounts.getOrDefault(firstLetter, 0) + 1);
            }
        }

        // Sort by letter (alphabetically) and return as LinkedHashMap to preserve order
        List<Map.Entry<String, Integer>> sortedEntries = new ArrayList<>(letterCounts.entrySet());
        sortedEntries.sort(Map.Entry.comparingByKey());

        // Use LinkedHashMap to maintain insertion order (alphabetical order)
        Map<String, Integer> result = new LinkedHashMap<>();
        for (Map.Entry<String, Integer> entry : sortedEntries) {
            result.put(entry.getKey(), entry.getValue());
        }

        return result;
    }
}
