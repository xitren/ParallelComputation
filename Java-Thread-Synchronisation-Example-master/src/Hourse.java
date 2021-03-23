import java.util.Random;

public class Hourse implements Runnable {
    private String name;
    private Race race;
    private WaterTrough trough;
    private Random rand = new Random(System.currentTimeMillis());

    public Hourse(String name, Race race, WaterTrough trough)
    {
        this.name = name;
        this.race = race;
        this.trough = trough;
    }

    /**
    * Called by each horse to run a lap.
    * @return the time it took to run a lap.
    * @throws InterruptedException
    */
    public long runLap() throws InterruptedException
    {
        long duration = Math.abs((this.rand.nextLong())) % 4000 + 2000;
        Thread.sleep(duration);
        return duration;
    }
    /**
    * This is the entry method for each 'Horse' thread we create
    */
    public void run()
    {
        try
        {
            this.race.getReadyToRace();
            System.out.println(this.name + " is off running!");

            for(int i=1; i<=3; i++){
                long time = this.runLap();
                System.out.println(name + " completes lap " + i + " in " + (double)time);

                time = this.trough.getDrink();
                System.out.println(name + " drink for " + (double)time/(double)1000.0);

            }
            int place = this.race.crossFinishLine();
            System.out.println(">>> " + name + " finishes in position " + place + "! <<<");
        } catch (InterruptedException ie) {
            System.out.println(name + " apparantly broke a leg and won't be finishing the race.");
        }
    }

}
