import java.io.*;
import java.util.*;

class Freq {
    String word;
    int freq;
    Freq(String w, int f) {
        word = w;
        freq = f;
    }
}

public class Main {

    static int tospace(int c) {
        if (!Character.isLetter(c)) {
            return ' ';
        } else {
            return c;
        }
    }

    static List<String> getStopWords() {
        List<String> stopWords = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader("../stop_words.txt"))) {
            StringBuilder buf = new StringBuilder();
            int ch;
            while ((ch = br.read()) != -1) {
                if (ch == ',') {
                    if (buf.length() > 0) {
                        stopWords.add(buf.toString());
                        buf.setLength(0);
                    }
                } else {
                    buf.append((char) ch);
                }
            }
            if (buf.length() > 0) {
                stopWords.add(buf.toString());
            }
        } catch(IOException e) {
            // handle or ignore
        }

        for (char c = 'a'; c <= 'z'; c++) {
            stopWords.add(Character.toString(c));
        }

        Collections.sort(stopWords);
        return stopWords;
    }

    static String readFile(String pathToFile) {
        StringBuilder sb = new StringBuilder();
        try (FileInputStream fis = new FileInputStream(pathToFile)) {
            int c;
            while ((c = fis.read()) != -1) {
                sb.append((char) c);
            }
        } catch(IOException e) {
            // handle or ignore
        }
        return sb.toString();
    }

    static String filterChars(String strData) {
        StringBuilder sb = new StringBuilder(strData.length());
        for (int i = 0; i < strData.length(); i++) {
            sb.append((char) tospace(strData.charAt(i)));
        }
        return sb.toString();
    }

    static String normalize(String strData) {
        return strData.toLowerCase(Locale.ROOT);
    }

    static List<String> scan(String strData) {
        List<String> words = new ArrayList<>();
        Scanner scanner = new Scanner(strData);
        while (scanner.hasNext()) {
            words.add(scanner.next());
        }
        return words;
    }

    static List<String> removeStopWords(List<String> words) {
        List<String> stopWords = getStopWords();
        List<String> filtered = new ArrayList<>();
        for (String w : words) {
            if (Collections.binarySearch(stopWords, w) < 0)
                filtered.add(w);
        }
        return filtered;
    }

    static Map<String, Integer> frequencies(List<String> words) {
        Map<String, Integer> freq = new HashMap<>();
        for (String w : words) {
            freq.put(w, freq.getOrDefault(w, 0) + 1);
        }
        return freq;
    }

    static List<Freq> sort(Map<String, Integer> wordFreq) {
        List<Freq> outList = new ArrayList<>();
        for (Map.Entry<String, Integer> entry : wordFreq.entrySet()) {
            outList.add(new Freq(entry.getKey(), entry.getValue()));
        }
        outList.sort((a, b) -> b.freq - a.freq);
        return outList;
    }

    public static void main(String[] args) {
        List<Freq> wordFreqs = sort(
            frequencies(
                removeStopWords(
                    scan(
                        normalize(
                            filterChars(
                                readFile(args[0])
                            )
                        )
                    )
                )
            )
        );
        for (int i = 0; i < Math.min(25, wordFreqs.size()); i++) {
            System.out.println(wordFreqs.get(i).word + " - " + wordFreqs.get(i).freq);
        }
    }
}