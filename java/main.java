import java.util.ArrayList;

public class Main {
    public static void main(String[] args) {
        System.out.println("Hello World");
        ArrayList<String> s = new ArrayList<String>();
        s.add("a");
        test(s);

        final int maxAlarm = 2;
        final int end = Math.min(maxAlarm, s.size());
        final var a = s.subList(0, end);
        for (String str : a) {
            System.out.println(str);
        }

    }

    public static void test(ArrayList<String> s)
    {
        final int maxAlarm = 3;
        for(int index = 0; index != s.size() && index < maxAlarm; ++index)
        {
            System.out.println(s.get(index));
        }


    }
}