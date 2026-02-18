import java.io.*;
import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        File sourceFile;
        File tempFile;

        while(true) {
            // asks the user for the name of the file they want to format, this is the source file.
            System.out.print("\nEnter a file name (.txt): ");
            String userInput = scanner.nextLine();

            // checks if the user input the file name in the correct format of: .txt
            // adds the .txt extension if it does not
            if (!userInput.endsWith(".txt")) {
                userInput += ".txt";
            }

            // checks if the source file exists and throws an exception if it does not
            sourceFile = new File(userInput);
            if (sourceFile.exists()) {
                System.out.println("Source file located: " + sourceFile);
            } else {
                System.out.println("The file does not exist: " + sourceFile);
                System.out.println("Try again.");
                continue;
            }
            System.out.println();

            // generates the temp file with a unique number id
            // the method generates a name and checks if a file with that name exists.
            // if the file exists already, it creates a new one and uses a different name.
            tempFile = generateUniqueTempFile();

            try {
                // create the temp file under the new name
                if (tempFile.createNewFile()) {
                    System.out.println("Temporary file created: " + tempFile.getName());
                } else {
                    System.out.println("Failed to create file.");
                }
            } catch (IOException e) {
                System.out.println("An error occurred while creating the temporary file.");
                break;
            }
//            System.out.println();

            // uses a buffered reader to read the contents of the source file
            // uses the buffered writer to copy the contents to the
            try (BufferedReader reader = new BufferedReader(new FileReader(sourceFile));
                 BufferedWriter writer = new BufferedWriter(new FileWriter(tempFile))) {

                String currentLine;

                // while the buffered reader is reading each line in the text file
                while ((currentLine = reader.readLine()) != null) {

                    // create an array of sentences where each line is split by periods "." and added to the array.
                    String[] sentences = currentLine.split("\\.");

                    // goes through each index in the array
                    for (String sentence : sentences) {
                        // if the index is not empty it will trim the String and write the String to the temp file.
                        if (!sentence.isEmpty()) {
                            writer.write(sentence.trim());
                            // writes the sentence on a new line
                            writer.newLine();
                        }
                    }
                }
                System.out.println(tempFile + " has been formatted.");

            } catch (IOException e) {
                System.out.println("An error occurred while processing the file: " + e.getMessage());
                break;
            }

            // uses the buffered reader to read the temp file
            // uses the buffered writer to write over the source file
            // reads the temp file and overwrites the formatted sentences back to the source file.
            try (BufferedReader reader = new BufferedReader(new FileReader(tempFile));
                 BufferedWriter writer = new BufferedWriter(new FileWriter(sourceFile))) {

                String currentLine;
                // while reading the temp, if the current line being read is not null, it will print that line to the sourceFile in a new line.
                while ((currentLine = reader.readLine()) != null) {
                    writer.write(currentLine);
                    writer.newLine();
                }
                System.out.println(sourceFile + " has been formatted.");

            } catch (IOException e) {
                System.out.println("An error occurred while processing the file: " + e.getMessage());
                break;
            }
//            System.out.println();

            // uses the delete file method if it returns true.
            if (deleteFile(tempFile.getName())) {
                System.out.println("Temporary file deleted successfully: " + tempFile.getName());
            } else {
                System.out.println("File not found or deletion failed: " + tempFile.getName());
            }
            break;
        }
    }

    // method to delete the temporary file once it has formatted the source file.
    // if the file exists and can be deleted it will return true
    public static boolean deleteFile(String filePath) {
        File file = new File(filePath);
        return file.exists() && file.delete();
    }

    // method to generate a unique temporary file
    private static File generateUniqueTempFile() {
        int fileCounter = 1;
        File tempFile;
        String baseName = "temp";
        String extension = ".txt";
        // do while loop to generate a file name with a number iterating, starting at 1
        do {
            String fileName = baseName + fileCounter + extension;
            tempFile = new File(fileName);
            fileCounter++;

        } while (tempFile.exists()); // repeats the loop if the file already exists
        return tempFile;
    }
}
