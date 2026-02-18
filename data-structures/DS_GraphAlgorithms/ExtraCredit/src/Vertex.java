public class Vertex implements Comparable<Vertex> {
    public String name;
    public int indegree;
    public int topNum;
    public int discovered;
    public int finished;
    public boolean processed;
    public Vertex pred;

    @Override
    public int hashCode() {
        return name.hashCode();
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Vertex)) return false; //
        Vertex another = (Vertex) o;
        return this.name.equals(another.name);
    }

    @Override
    public String toString() {
        return name;
//        return name + " w/ topNum/ discovered/ finished: " + topNum + "/ " + discovered + "/ " + finished;
    }


    public Vertex(String name) {
        this.name = name;
        this.indegree = 0;
        this.discovered = 0;
        this.finished = 0; //
        this.processed = false;
    }

    public int compareTo(Vertex another) {
        return topNum - another.topNum;
    }
}