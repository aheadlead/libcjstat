import java.util.*;

public class bomb {
    public static void main(String[] args) {
        List<Integer> L = new ArrayList<Integer>();
        while (true) {
            try {
                L.add(233);
            } catch (java.lang.OutOfMemoryError e) {
                L = null;
                System.gc();
                L = new ArrayList<Integer>();
            }
        }
    }
}
