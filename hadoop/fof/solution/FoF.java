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
//  Notes:
//    Uses NullWritable because there is no value data
//    Does not use a combiner class, because we need all
//      data from the same key in the reducer iterator to 
//      count it.
//    Trades off more computation/network for more parallelism.
//      Each mapper proposes all possible FoF relationships.
//      Many of these don't suceed.
//    Needs CLI options installed for Java.

public class FoF
{
  public static class FoFMap extends Mapper<Object, Text, Text, NullWritable >
  {
    private final static NullWritable nullw = NullWritable.get();

    public void map(Object key, Text value, Context context ) 
        throws IOException, InterruptedException
    {
      // Acquire the line from the mapper
      String line = value.toString();

      // Tokenize the line
      StringTokenizer tokenizer = new StringTokenizer(line);

      // Get the ID of the person
      long id = Long.parseLong ( tokenizer.nextToken()); 

      // Build an array of friends
      int arsize = tokenizer.countTokens();
      long[] friends = new long [ arsize ]; 

      for ( int i=0; i<arsize; i++ )
      {
        friends[i] = Long.parseLong ( tokenizer.nextToken());
      }

      // Output all possible triples of friends. 
      //   Propose yourself and B to A, yourself and A to B
      for ( int i=0; i<arsize-1; i++ )
      {
        for ( int j=i+1; j<arsize; j++ )
        {
          Text outputkey;

          if ( id < friends[i] )
          {   
            outputkey = new Text ( Long.toString(friends[j]) + " " + Long.toString(id) + " " +  Long.toString(friends[i]));
          } 
          else
          {
            outputkey = new Text ( Long.toString(friends[j]) + " " + Long.toString(friends[i]) + " " +  Long.toString(id));
          }
          context.write( outputkey, nullw );
//          System.out.println ( id + ": Map output key: " + outputkey ); 

          if ( id < friends[j] )
          {   
            outputkey = new Text ( Long.toString(friends[i]) + " " + Long.toString(id) + " " +  Long.toString(friends[j]));
          } 
          else
          {
            outputkey = new Text ( Long.toString(friends[i]) + " " + Long.toString(friends[j]) + " " +  Long.toString(id));
          }
          context.write( outputkey, nullw );
//          System.out.println ( id + ": Map output key: " + outputkey ); 
        }
      }
    }
  }

  public static class FoFReduce extends Reducer<Text, NullWritable, Text, NullWritable> 
  {
    // Create the output NullWritable object object per class 
    private NullWritable result = NullWritable.get();

    // Implement the reducer
    public void reduce(Text key, Iterable<NullWritable> values, Context context) 
        throws IOException, InterruptedException
    {
      int sum = 0;

      for ( NullWritable val : values )
      {
        sum += 1;
      }
      
      // If both partners propose the same trie, the FoF relationship is valid.
      if ( sum >= 2 )
      {
        context.write(key,result);
        System.out.println ( "Found triple " + key );
      }
      else
      {
//        System.out.println ( "Failed FoF proposal " + key );
      }
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
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(NullWritable.class);
    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
