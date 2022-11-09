import java.io.IOException;
import java.util.*;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.*;

// 
// Implements the Friend of a Friend relationship 
//
//  Outputs FoF for each ID, i.e. ID, Friend1, Friend2
//

public class FoF
{
  public static class FoFMap extends Mapper< ...TODO... >
  {

    public void map(TODOTYPE key, TODOTYPE value, Context context ) 
        throws IOException, InterruptedException
    {
      // Acquire the line from the mapper
      String line = value.toString();

      // Tokenize the line
      StringTokenizer tokenizer = new StringTokenizer(line);

      // Get the ID of the person
      long id = Long.parseLong ( tokenizer.nextToken()); 

      // Get the list of friends
      int arsize = tokenizer.countTokens();
      long[] friends = new long [ arsize ]; 
      for ( int i=0; i<arsize; i++ )
      {
        friends[i] = Long.parseLong ( tokenizer.nextToken());
      }

      //  TODO

          // at some point
          context.write(...TODO...);
  }

  public static class FoFReduce extends Reducer<...TODO...> 
  {
    // Implement the reducer
    public void reduce(TODOTYPE key, Iterable<TODOTYPE> values, Context context) 
        throws IOException, InterruptedException
    {
        TODO
    }
  }

  public static void main(String[] args) throws Exception 
  {
    Configuration conf = new Configuration();

//  RB Notice no combiner class
    Job job = Job.getInstance(conf, "FoF");
    job.setJarByClass(FoF.class);
    job.setMapperClass(FoFMap.class);
    job.setReducerClass(FoFReduce.class);
    job.setOutputKeyClass(TODOTYPE.class);
    job.setOutputValueClass(TODOTYPE.class);
    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
