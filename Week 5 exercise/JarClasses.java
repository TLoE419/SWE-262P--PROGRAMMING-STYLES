import java.io.IOException;
import java.io.File;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.lang.reflect.Field;
import java.util.Enumeration;

public class JarClasses {

    public static void main(String[] args)
    {
        if(args.length == 0)
        {
            System.out.println("Usage: java JarClasses <path-to-jar-file>");
            return;
        }

        String jarFilePath = args[0];

        try(JarFile jarFile = new JarFile(jarFilePath); URLClassLoader classLoader = new URLClassLoader(new URL[]{new File(jarFilePath).toURI().toURL()}))
        {

            Enumeration<JarEntry> entries = jarFile.entries();
            while(entries.hasMoreElements())
            {
                JarEntry entry = entries.nextElement();
                
                if(entry.getName().endsWith(".class") && !entry.isDirectory())
                {
                    String className = entry.getName().replace(".class", "").replace("/", ".");
                    Class<?> myClass = classLoader.loadClass(className);

                    Method[] methods = myClass.getDeclaredMethods();
                    Field[] fields = myClass.getDeclaredFields();

                    int publicCount = 0;
                    int privateCount = 0;
                    int protectedCount = 0;
                    int staticCount = 0;
                    int fieldCount = 0;

                    for(Method method : methods)
                    {
                        
                        int modifiers = method.getModifiers();
                        if(Modifier.isPublic(modifiers)) publicCount++;
                        if(Modifier.isPrivate(modifiers)) privateCount++;
                        if(Modifier.isProtected(modifiers)) protectedCount++;
                        if(Modifier.isStatic(modifiers)) staticCount++;
                    }

                    fieldCount = fields.length;

                    System.out.println("----------" + className + "----------");
                    System.out.println("  Public methods: " + publicCount);
                    System.out.println("  Private methods: " + privateCount);
                    System.out.println("  Protected methods: " + protectedCount);
                    System.out.println("  Static methods: " + staticCount);
                    System.out.println("  Fields: " + fieldCount);
                }
                
                
            }
        }
        catch(IOException | ClassNotFoundException e)
        {
            System.err.print("Error: " + e.getMessage());
        }
    }
}
