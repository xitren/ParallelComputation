public class HourseRaceMain {
    /**
    * @param args
    */
    public static void main(String[] args) throws InterruptedException
    {
        // Create a race and water trough for the tursty race hources.
        Race race = new Race();
        WaterTrough through = new WaterTrough();

        // spin off four eager hourses!
        new Thread(new Hourse("Hourse 1", race, through)).start();
        new Thread(new Hourse("Hourse 2", race, through)).start();
        new Thread(new Hourse("Hourse 3", race, through)).start();
        new Thread(new Hourse("Hourse 4", race, through)).start();

        // Wait to make sure each thread gets off to the finish line...
        System.out.print("Get ready ...");
        Thread.sleep(2000);
        System.out.print("get sit ...");
        Thread.sleep(2000);
        System.out.println("Go!");
        race.startRace();
    }
}
