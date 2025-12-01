import java.io.*;
import java.util.*;
import java.net.*;
import java.lang.reflect.*;

public class twenty 
{
    public static void main(String[] args) 
    {
        if(args.length < 1)
        {
            System.out.println("Usage: java Twenty <path_to_text_file>");
            System.exit(1);
        }

        String textFile = args[0];

        try
        {
            // Load configuration
            Properties config = new Properties();
            config.load(new FileInputStream("config.properties"));

            // Get properties string
            String extractorClass = config.getProperty("extractor");
            String counterClass = config.getProperty("counter");

            // Load plugin into a Object
            Object extractor = loadPlugin(extractorClass);
            Object counter = loadPlugin(counterClass);

            // Find a Method in Object and call it
            Method extractMethod = extractor.getClass().getMethod("extractWords", String.class);
            List<String> words = (List<String>) extractMethod.invoke(extractor, textFile);

            Method countMethod = counter.getClass().getMethod("count", List.class);
            Map<String, Integer> wordFreqs = (Map<String, Integer>) countMethod.invoke(counter, words);

            printResults(wordFreqs);
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }

    // Load a plugin
    private static Object loadPlugin(String className) throws Exception
    {
        URLClassLoader classLoader = URLClassLoader.newInstance
        (
            new URL[] { new File("plugins/").toURI().toURL() }
        );
        Class<?> cls = Class.forName(className, true, classLoader);
        return cls.getDeclaredConstructor().newInstance();
    }

    // Print Top 25
    private static void printResults(Map<String, Integer> wordFreqs)
    {
        int count = 0;
        for(Map.Entry<String, Integer> entry : wordFreqs.entrySet())
        {
            if(count >= 25) break;
            System.out.println(entry.getKey() + " - " + entry.getValue());
            count++;
        }
    }
}
