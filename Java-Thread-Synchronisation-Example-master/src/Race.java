import java.util.Random;

public class Race {
    public int rank = 0;

    /**
    * Called by each horse to line up the starting line.
    * @throws InterruptedException
    */
    public synchronized void getReadyToRace() throws InterruptedException
    {
        this.wait();
    }

    /**
    * Called to start the race
    */
    public synchronized void startRace()
    {
        this.notifyAll();
    }
    /**
    * Called by each horse to cross the finish line.
    */
    public synchronized int crossFinishLine()
    {
        return ++rank;
    }

}
