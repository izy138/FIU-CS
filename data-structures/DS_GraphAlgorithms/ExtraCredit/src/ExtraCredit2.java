import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;
import java.util.Scanner;

public class ExtraCredit2 {

    public static class BellmanFord {
        private static final double INF = Double.POSITIVE_INFINITY;

        // Bellman-Ford algorithm
        public static double[] bellmanFord(double[][] graph, int source) {
            int n = graph.length;
            double[] dist = new double[n];
            Arrays.fill(dist, INF);
            dist[source] = 0;

            // relaxes all edges (n-1) times
            for (int i = 0; i < n - 1; i++) {
                for (int u = 0; u < n; u++) {
                    for (int v = 0; v < n; v++) {
                        if (graph[u][v] != INF && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                            dist[v] = dist[u] + graph[u][v];
                        }
                    }
                }
            }

            return dist;
        }

        // prints the most profitable exchange sequence
        public static void printResults(double[] dist, String[] currencies, String sourceCurrency, double[][] exchangeRates, int sourceIndex) {
            System.out.println("Source currency is " + sourceCurrency);
            for (int i = 0; i < dist.length; i++) {
                //calculates the max exchange rate from dist array
                double maxExchangeRate = Math.exp(-dist[i]);  // Correct max exchange rate calculation (exp(-dist))

                // direct rate from the exchangeRates matrix
                double directRate = exchangeRates[sourceIndex][i];

                System.out.println(currencies[i] + ": max Exchange Rate is " + maxExchangeRate + ", and direct rate is " + directRate);
            }
        }
    }

    public static class ExchangeRate {
        String[] currencyNames;
        double[][] exchangeRates;

        public ExchangeRate(String[] currencyNames, double[][] exchangeRates) {
            this.currencyNames = currencyNames;
            this.exchangeRates = exchangeRates;
        }
    }

    public static class Main {
        public static void main(String[] args) {
            Scanner scanner = new Scanner(System.in);
            //reads the exchange rates for the csv
            try {
                ExchangeRate exchangeRateData = readExchangeRatesFromFile("src/exchangerates.csv");

                // allows user input:
                System.out.print("Enter the source currency: ");
                String sourceCurrency = scanner.nextLine().trim();

                // finds the index of the users input currency
                int sourceIndex = -1;
                for (int i = 0; i < exchangeRateData.currencyNames.length; i++) {
                    if (exchangeRateData.currencyNames[i].equalsIgnoreCase(sourceCurrency)) {
                        sourceIndex = i;
                        break;
                    }
                }

                if (sourceIndex == -1) {
                    System.out.println("Currency not found. Try again.");
                    return;
                }

                // create the graph with -log(exchangeRate) as weight
                double[][] graph = new double[exchangeRateData.currencyNames.length][exchangeRateData.currencyNames.length];
                for (int i = 0; i < graph.length; i++) {
                    for (int j = 0; j < graph.length; j++) {
                        if (exchangeRateData.exchangeRates[i][j] > 0) {
                            graph[i][j] = -Math.log(exchangeRateData.exchangeRates[i][j]);
                        } else {
                            graph[i][j] = Double.POSITIVE_INFINITY;
                        }
                    }
                }

                // use BellmanFord alg to find the maximum exchange rate
                BellmanFord bf = new BellmanFord();
                double[] dist = bf.bellmanFord(graph, sourceIndex);

                bf.printResults(dist, exchangeRateData.currencyNames, sourceCurrency, exchangeRateData.exchangeRates, sourceIndex);

            } catch (IOException e) {
                System.err.println("Error reading CSV file: " + e.getMessage());
                e.printStackTrace();
            } finally {
                scanner.close();
            }
        }

// buffered reader for the csv file
        public static ExchangeRate readExchangeRatesFromFile(String filename) throws IOException {
            List<String> currencyNamesList = new ArrayList<>();
            List<double[]> exchangeRatesList = new ArrayList<>();

            BufferedReader reader = new BufferedReader(new FileReader(filename));
            String line;
            boolean firstLine = true;
            String[] headers = null;

            while ((line = reader.readLine()) != null && !line.trim().isEmpty()) {
                //disrequard the header
                if (firstLine) {
                    headers = line.split(",");
                    firstLine = false;
                    continue;
                }

                String[] parts = line.split(",");
                if (parts.length >= 2) {
                    // gets the currency name from the first column
                    currencyNamesList.add(parts[0].trim());

                    // goes through the exchange rates for the currency
                    double[] exchangeRates = new double[parts.length - 1];
                    for (int i = 1; i < parts.length; i++) {
                        try {
                            exchangeRates[i - 1] = Double.parseDouble(parts[i].trim());
                        } catch (NumberFormatException e) {
                            exchangeRates[i - 1] = 0.0;
                        }
                    }
                    exchangeRatesList.add(exchangeRates);
                }
            }
            reader.close();

            // adds the currency names to an array
            String[] currencies = currencyNamesList.toArray(new String[0]);

            // adds the exchange rates to a 2d array matrix
            double[][] exchangeRatesArray = new double[exchangeRatesList.size()][];
            exchangeRatesList.toArray(exchangeRatesArray);

            return new ExchangeRate(currencies, exchangeRatesArray);
        }
    }
}